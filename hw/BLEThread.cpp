
#include "hw/BLEThread.h"
#include "hw/HWInputButtonBtGPIO.h"
#include "util/Config.h"
#include "util/Debug.h"

#include <QDomDocument>

/*******************************************************
 * D-Bus Helper functions
 *******************************************************/


#include <glib.h>
#include <gio/gio.h>
#include <glib/gprintf.h>

GDBusConnection* bluez_init()
{
    GError* error = NULL;

    g_type_init();

    GDBusConnection* conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM,
                                          NULL,
                                          &error);
    if(conn == NULL)
    {
        g_printf("g_bus_get_sync failed\n");

        g_printerr("Error invoking g_bus_get_sync: %s\n", error->message);
    }

    return conn;
}

void bluez_cleanup(GDBusConnection* conn)
{
    g_object_unref(conn);
}

// returns object path of default adapter or NULL if failed
// this object path must be freed with g_free()
gchar* bluez_get_default_adapter(GDBusConnection* conn)
{
    GError* error = NULL;

    GDBusMessage* call_message = g_dbus_message_new_method_call("org.bluez",
                                                       "/",
                                                       "org.bluez.Manager",
                                                       "DefaultAdapter");
    if(call_message == NULL)
    {
        g_printf("g_dbus_message_new_method_call failed\n");

        return NULL;
    }

    GDBusMessage* reply_message = g_dbus_connection_send_message_with_reply_sync(conn,
                                                                          call_message,
                                                                          G_DBUS_SEND_MESSAGE_FLAGS_NONE,
                                                                          -1,
                                                                          NULL,
                                                                          NULL,
                                                                          &error);
    if(reply_message == NULL)
    {
        g_printf("g_dbus_connection_send_message_with_reply_sync failed\n");

        return NULL;
    }

    if(g_dbus_message_get_message_type(reply_message) == G_DBUS_MESSAGE_TYPE_ERROR)
    {
        printf("Error occured\n");

        g_dbus_message_to_gerror(reply_message, &error);
        g_printerr("Error invoking g_dbus_connection_send_message_with_reply_sync: %s\n", error->message);

        g_error_free(error);

        return NULL;
    }


    GVariant* variant = g_dbus_message_get_body(reply_message);

    // get first child, as this is the object path of the default interface of bluez
    GVariant* var_child = g_variant_get_child_value(variant, 0);

    const gchar* tmp_path = g_variant_get_string(var_child, NULL);

    // copy content of tmp_path to obj_path, as tmp_path gets freed after unref of the variant
    gchar* obj_path = g_strdup(tmp_path);

    // cleanup
    g_variant_unref(var_child);

    g_object_unref(call_message);
    g_object_unref(reply_message);

    return obj_path;
}


// returns the object path to a given device or NULL if failed
// object path has to be freed with g_free()
gchar* bluez_find_device(GDBusConnection* conn, const gchar* adapter_path, const char* bt_addr)
{
    GError* error = NULL;

    GDBusMessage* call_message = g_dbus_message_new_method_call("org.bluez",
                                                       adapter_path,
                                                       "org.bluez.Adapter",
                                                       "FindDevice");
    if(call_message == NULL)
    {
        return NULL;
    }

    GVariant* variant_addr = g_variant_new_string(bt_addr);
    GVariant* variant_body = g_variant_new_tuple(&variant_addr, 1);

    g_dbus_message_set_body(call_message, variant_body);

    GDBusMessage* reply_message = g_dbus_connection_send_message_with_reply_sync(conn,
                                                                          call_message,
                                                                          G_DBUS_SEND_MESSAGE_FLAGS_NONE,
                                                                          -1,
                                                                          NULL,
                                                                          NULL,
                                                                          &error);
    if(reply_message == NULL)
    {
        return NULL;
    }

    if(g_dbus_message_get_message_type(reply_message) == G_DBUS_MESSAGE_TYPE_ERROR)
    {
        return NULL;
    }

    GVariant* variant = g_dbus_message_get_body(reply_message);

    // get first child, as this is the object path of the default interface of bluez
    GVariant* var_child = g_variant_get_child_value(variant, 0);

    const gchar* tmp_path = g_variant_get_string(var_child, NULL);

    // copy content of tmp_path to obj_path, as tmp_path gets freed after unref of the variant
    gchar* obj_path = g_strdup(tmp_path);

    // cleanup
    g_variant_unref(var_child);

    g_object_unref(call_message);
    g_object_unref(reply_message);

    return obj_path;
}


GVariant* bluez_characteristic_get_value(GDBusConnection* conn, const gchar* char_path)
{
    GError* error = NULL;

    GDBusMessage* call_message = g_dbus_message_new_method_call("org.bluez",
                                                       char_path,
                                                       "org.bluez.Characteristic",
                                                       "GetProperties");
    if(call_message == NULL)
    {
        g_printf("g_dbus_message_new_method_call failed\n");

        return NULL;
    }

    GDBusMessage* reply_message = g_dbus_connection_send_message_with_reply_sync(conn,
                                                                          call_message,
                                                                          G_DBUS_SEND_MESSAGE_FLAGS_NONE,
                                                                          -1,
                                                                          NULL,
                                                                          NULL,
                                                                          &error);
    if(reply_message == NULL)
    {
        g_printf("g_dbus_connection_send_message_with_reply_sync failed\n");

        return NULL;
    }

    if(g_dbus_message_get_message_type(reply_message) == G_DBUS_MESSAGE_TYPE_ERROR)
    {
        g_printf("Error occured\n");

        g_dbus_message_to_gerror(reply_message, &error);
        g_printerr("Error invoking g_dbus_connection_send_message_with_reply_sync: %s\n", error->message);

        g_error_free(error);

        return NULL;
    }



    GVariant* var_body = g_dbus_message_get_body(reply_message);

    g_variant_ref(var_body);

    // cleanup
    g_object_unref(call_message);
    g_object_unref(reply_message);

    return var_body;
}


void bluez_register_watcher(GDBusConnection* conn, const gchar* service_path)
{
    GError* error = NULL;

    GDBusMessage* call_message = g_dbus_message_new_method_call("org.bluez",
                                                       service_path,
                                                       "org.bluez.Characteristic",
                                                       "RegisterCharacteristicsWatcher");
    if(call_message == NULL)
    {
        printf("g_dbus_message_new_method_call failed\n");

        return;
    }

    gchar path[255];
    g_snprintf(path, 255, "/test/bluez/%d", getpid());

    GVariant* variant_addr = g_variant_new_object_path(path);
    GVariant* variant_body = g_variant_new_tuple(&variant_addr, 1);

    g_dbus_message_set_body(call_message, variant_body);

    GDBusMessage* reply_message = g_dbus_connection_send_message_with_reply_sync(conn,
                                                                          call_message,
                                                                          G_DBUS_SEND_MESSAGE_FLAGS_NONE,
                                                                          -1,
                                                                          NULL,
                                                                          NULL,
                                                                          &error);
    if(reply_message == NULL)
    {
        printf("g_dbus_connection_send_message_with_reply_sync failed\n");

        return;
    }

    if(g_dbus_message_get_message_type(reply_message) == G_DBUS_MESSAGE_TYPE_ERROR)
    {
        printf("Error occured\n");

        g_dbus_message_to_gerror(reply_message, &error);
        g_printerr("Error invoking g_dbus_connection_send_message_with_reply_sync: %s\n", error->message);

        g_error_free(error);

        return;
    }

    // cleanup
    g_object_unref(call_message);
    g_object_unref(reply_message);
}

void bluez_unregister_watcher(GDBusConnection* conn, const gchar* service_path)
{
    GError* error = NULL;

    GDBusMessage* call_message = g_dbus_message_new_method_call("org.bluez",
                                                       service_path,
                                                       "org.bluez.Characteristic",
                                                       "UnregisterCharacteristicsWatcher");
    if(call_message == NULL)
    {
        printf("g_dbus_message_new_method_call failed\n");

        return;
    }

    gchar path[255];
    g_snprintf(path, 255, "/test/bluez/%d", getpid());

    GVariant* variant_addr = g_variant_new_object_path(path);
    GVariant* variant_body = g_variant_new_tuple(&variant_addr, 1);

    g_dbus_message_set_body(call_message, variant_body);

    GDBusMessage* reply_message = g_dbus_connection_send_message_with_reply_sync(conn,
                                                                          call_message,
                                                                          G_DBUS_SEND_MESSAGE_FLAGS_NONE,
                                                                          -1,
                                                                          NULL,
                                                                          NULL,
                                                                          &error);
    if(reply_message == NULL)
    {
        printf("g_dbus_connection_send_message_with_reply_sync failed\n");

        return;
    }

    if(g_dbus_message_get_message_type(reply_message) == G_DBUS_MESSAGE_TYPE_ERROR)
    {
        printf("Error occured\n");

        g_dbus_message_to_gerror(reply_message, &error);
        g_printerr("Error invoking g_dbus_connection_send_message_with_reply_sync: %s\n", error->message);

        g_error_free(error);

        return;
    }

    // cleanup
    g_object_unref(call_message);
    g_object_unref(reply_message);
}

/*******************************************************************************************************
 * Handle disconnects
 *******************************************************************************************************/

static void
bluez_handle_disconnect(GDBusConnection       *connection,
                    const gchar           *sender,
                    const gchar           *object_path,
                    const gchar           *interface_name,
                    const gchar           *signal_name,
                    GVariant              *parameters,
                    gpointer               user_data)
{
    // check if parameter is valid and is the parameter we were looking for
    if( !(g_variant_type_equal(g_variant_get_type(parameters), "(sv)") &&
          g_variant_n_children(parameters) == 2) )
        return;

    GVariant* name = g_variant_get_child_value(parameters, 0);
    GVariant* value = g_variant_get_variant(g_variant_get_child_value(parameters, 1));

    if( !(g_strcmp0(g_variant_get_string(name, NULL), "Connected") == 0 &&
          g_variant_type_equal(g_variant_get_type(value), G_VARIANT_TYPE_BOOLEAN) ) )
        return;

    gboolean connected = g_variant_get_boolean(value);
    g_printf("Connected: %d\n", connected);

    if(!connected)
    {
        gchar* device_path = (gchar*)user_data;
        gchar* service_path = g_strconcat(device_path, "/service0007", NULL);

        bluez_unregister_watcher(connection, service_path);
        bluez_register_watcher(connection, service_path);
    }
}

guint bluez_register_disconnect_handler(GDBusConnection* conn, char* device_path)
{
    guint signal_id = g_dbus_connection_signal_subscribe(conn,
                                                         "org.bluez",
                                                         "org.bluez.Device",
                                                         "PropertyChanged",
                                                         device_path,
                                                         "Connected",
                                                         G_DBUS_SIGNAL_FLAGS_NONE,
                                                         bluez_handle_disconnect,
                                                         device_path,
                                                         NULL);

    if(signal_id <= 0)
    {
        g_printerr("g_dbus_connection_signal_subscribe failed: %d\n", signal_id);
    }

    return signal_id;
}


/*******************************************************************************************************
 * Watcher
 *******************************************************************************************************/


/* ---------------------------------------------------------------------------------------------------- */
static GDBusNodeInfo *introspection_data = NULL;

/* Introspection data for the service we are exporting */
static const gchar introspection_xml[] =
    "<node>"
    "  <interface name='org.bluez.Watcher'>"
    "    <method name='ValueChanged'>"
    "      <arg type='o' name='characteristic' direction='in' />"
    "      <arg type='ay' name='value' direction='in' />"
    "    </method>"
    "  </interface>"
    "  <interface name='org.bluez.Watcher'>"
    "    <method name='ValueChanged'>"
    "      <arg type='o' name='characteristic' direction='in' />"
    "      <arg type='ay' name='value' direction='in' />"
    "    </method>"
    "  </interface>"
  "</node>";
/* ---------------------------------------------------------------------------------------------------- */

struct bluez_user_data
{
    std::function<void (unsigned char)> callback_func;
};

static void
handle_method_call (GDBusConnection       *connection,
                    const gchar           *sender,
                    const gchar           *object_path,
                    const gchar           *interface_name,
                    const gchar           *method_name,
                    GVariant              *parameters,
                    GDBusMethodInvocation *invocation,
                    gpointer               user_data)
{
    if(g_strcmp0(method_name, "ValueChanged") == 0)
    {
        for(unsigned int j = 0; j < g_variant_n_children(parameters); j++)
        {
            GVariant* var_child = g_variant_get_child_value(parameters, j);

            if( g_variant_type_equal(g_variant_get_type(var_child), G_VARIANT_TYPE_OBJECT_PATH) )
                g_printf("%s\n", g_variant_get_string(var_child, NULL));
            else if( g_variant_type_equal(g_variant_get_type(var_child), "ay") )
            {
                BLEThread* thread = (BLEThread*)user_data;
                GVariant* var_state = g_variant_get_child_value(var_child, 0);

                thread->setState(g_variant_get_byte(var_state));


                for(unsigned int i = 0; i < g_variant_n_children(var_child); i++)
                {
                    GVariant* var_inner_child = g_variant_get_child_value(var_child, i);

                    if( g_variant_type_equal(g_variant_get_type(var_inner_child), G_VARIANT_TYPE_BYTE) )
                        g_printf("%02x ", g_variant_get_byte(var_inner_child));
                }

                g_printf("\n");
            }

            g_variant_unref(var_child);
        }
    }
    else
    {
        g_printf("Received method with name %s\n", method_name);
    }
}

static GVariant *
handle_get_property (GDBusConnection  *connection,
                     const gchar      *sender,
                     const gchar      *object_path,
                     const gchar      *interface_name,
                     const gchar      *property_name,
                     GError          **error,
                     gpointer          user_data)
{
    return NULL;
}

static gboolean
handle_set_property (GDBusConnection  *connection,
                     const gchar      *sender,
                     const gchar      *object_path,
                     const gchar      *interface_name,
                     const gchar      *property_name,
                     GVariant         *value,
                     GError          **error,
                     gpointer          user_data)
{
    return FALSE;
}

/* for now */
static const GDBusInterfaceVTable interface_vtable =
{
    handle_method_call,
    handle_get_property,
    handle_set_property
};


/* ---------------------------------------------------------------------------------------------------- */
static void on_name_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
    // do nothing as everything is okay
}

static void on_name_lost (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
    g_printerr("Could not get a name on the bus\n");
    //g_main_loop_quit();
}

guint bluez_create_watcher(GDBusConnection* conn, BLEThread* thread)
{
    /* We are lazy here - we don't want to manually provide
    * the introspection data structures - so we just build
    * them from XML.
    */
    introspection_data = g_dbus_node_info_new_for_xml (introspection_xml, NULL);
    g_assert(introspection_data != NULL);

    guint registration_id;

    gchar path[255];
    g_snprintf(path, 255, "/test/bluez/%d", getpid());

    registration_id = g_dbus_connection_register_object(conn,
                                                       path,
                                                       introspection_data->interfaces[0],
                                                       &interface_vtable,
                                                       thread,  /* user_data */
                                                       NULL,  /* user_data_free_func */
                                                       NULL); /* GError** */
    g_assert(registration_id > 0);

    return g_bus_own_name_on_connection(conn,
                                     "org.test.bluez",
                                     G_BUS_NAME_OWNER_FLAGS_NONE,
                                     on_name_acquired,
                                     on_name_lost,
                                     NULL,
                                     NULL);
}

void bluez_destroy_watcher(guint owner_id)
{
    g_bus_unown_name(owner_id);
}

unsigned char bluez_characteristic_extract_value(GVariant* variant)
{
    GVariant* var_array = g_variant_get_child_value(variant, 0);

    for(unsigned int i = 0; i < g_variant_n_children(var_array); i++)
    {
        GVariant* var_child = g_variant_get_child_value(var_array, i);

        if(g_variant_n_children(var_child) == 2)
        {
            GVariant* var_id = g_variant_get_child_value(var_child, 0);

            if(g_variant_type_equal(g_variant_get_type(var_id), G_VARIANT_TYPE_STRING) &&
               g_strcmp0(g_variant_get_string(var_id, NULL), "Value") == 0)
            {
                GVariant* inner_var = g_variant_get_child_value(var_child, 1);

                if( g_variant_type_equal(g_variant_get_type(inner_var), G_VARIANT_TYPE_VARIANT) )
                {
                    GVariant* var_value = g_variant_get_variant(inner_var);
                    if( g_variant_type_equal(g_variant_get_type(var_value), G_VARIANT_TYPE_BYTESTRING) )
                    {
                        GVariant* var_byte = g_variant_get_child_value(var_value, 0);
                        return g_variant_get_byte(var_byte);
                    }
                }
            }
        }

        g_variant_unref(var_child);
    }

    return 0;
}

/*******************************************************
 * BLEThread implementation
 *******************************************************/

BLEThread::BLEThread()
{
    m_loop = NULL;
}

BLEThread::~BLEThread()
{
    if(m_thread != 0)
        this->kill();
}

/**
 * @brief BTClassicThread::start starts this bluetooth thread.
 * The thread is automatically killed as soon as it is deleted, or it can be killed manually by BTClassicThread::kill
 */
void
BLEThread::start()
{
    pi_assert(m_thread == 0);

    // create and run thread
    pthread_create(&m_thread, NULL, BLEThread::run_internal, (void*)this);
}

/**
 * @brief BTClassicThread::kill kills this thread.
 */
void
BLEThread::kill()
{
    // TODO: FIXME, thread does not yet exit
    g_main_loop_quit(m_loop);

    pthread_join(m_thread, NULL);
    m_thread = 0;
}

void*
BLEThread::run_internal(void* arg)
{
    BLEThread* thread = (BLEThread*)arg;
    thread->run();

    return NULL;
}

BTThread*
BLEThread::load(QDomElement *root)
{
    return new BLEThread();
}

QDomElement
BLEThread::save(QDomElement *root, QDomDocument *document)
{
    QDomElement output = BTThread::save(root, document);

    QDomElement lowEnergy = document->createElement("LowEnergy");

    output.appendChild(lowEnergy);

    return output;
}

void
BLEThread::addGPInput(HWInputButtonBtGPIO* hw)
{
    GPInput gp;
    gp.hw = hw;
    gp.pin = hw->getPin();
    gp.pinGroup = hw->getPinGroup();

    // we cannot handle other pins then those in pingroup 2 and only pins from 0 to 3
    if(gp.pinGroup != 2 || gp.pin > 3)
        return;

    m_listGPInput.push_back(gp);
}

void
BLEThread::removeGPInput(HWInputButtonBtGPIO* hw)
{
    for(std::list<GPInput>::iterator it = m_listGPInput.begin(); it != m_listGPInput.end(); it++)
    {
        if( (*it).hw == hw)
        {
            m_listGPInput.erase(it);
            break;
        }
    }
}

void
BLEThread::setState(unsigned char state)
{
    pi_message("New state: %d", state);

    for(std::list<GPInput>::iterator it = m_listGPInput.begin(); it != m_listGPInput.end(); it++)
    {
        if( (*it).pinGroup == 2 )
        {
            (*it).hw->setValue( (state & (1 << (*it).pin)) == 0);
        }
    }
}

void
BLEThread::run()
{
    // initialize everything
    GDBusConnection* conn = bluez_init();
    if(conn == NULL)
    {
        pi_warn("Could not connect to bus");
        return;
    }

    gchar* adapter_path = bluez_get_default_adapter(conn);
    if(adapter_path == NULL)
    {
        pi_warn("Could not get default adapter");
        return;
    }

    gchar* device_path = bluez_find_device(conn, adapter_path, m_btaddr.c_str());
    if(device_path == NULL)
    {
        pi_warn("Could not get device path for the given bluetooth device");
        return;
    }

    // we want to take a look at only one special characteristic
    gchar* service_path = g_strconcat(device_path, "/service0007", NULL);
    gchar* char_path = g_strconcat(service_path, "/characteristic0009", NULL);

    GVariant* var = bluez_characteristic_get_value(conn, char_path);
    if(var != NULL)
    {
        unsigned char buttonState = bluez_characteristic_extract_value(var);

        this->setState(buttonState);

        g_variant_unref(var);
    }

    // register disconnect handler
    bluez_register_disconnect_handler(conn, device_path);

    // register watcher
    guint owner_id = bluez_create_watcher(conn, this);
    bluez_register_watcher(conn, service_path);



    // start runloop, will only exit, if the thread will be killed soon
    m_loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(m_loop);



    // cleanup
    bluez_destroy_watcher(owner_id);

    // TODO: Unsubscribe the disconnect handler

    g_free(char_path);
    g_free(service_path);
    g_free(device_path);
    g_free(adapter_path);

    bluez_cleanup(conn);
}


/*******************************************************
 * Virtual methods, not yet implemented
 *******************************************************/

void
BLEThread::addInput(BTI2CPolling* hw, unsigned int freq)
{
    pi_warn("Not yet implemented");
}

void
BLEThread::removeInput(BTI2CPolling* hw)
{
    pi_warn("Not yet implemented");
}

void
BLEThread::addOutput(std::function<void (BTThread*)> func)
{
    pi_warn("Not yet implemented");
}

void
BLEThread::addInputPCF8575(HWInput* hw, int slaveAddress, unsigned int port)
{
    pi_warn("Not yet implemented");
}

void
BLEThread::removeInputPCF8575(HWInput* hw, int slaveAddress)
{
    pi_warn("Not yet implemented");
}

void
BLEThread::addOutputPCF8575(HWOutput* hw, int slaveAddress, unsigned int port)
{
    pi_warn("Not yet implemented");
}

void
BLEThread::removeOutputPCF8575(HWOutput* hw, int slaveAddress)
{
    pi_warn("Not yet implemented");
}

// ATTENTION: USE ONLY IN BTTHREAD!!!!
void
BLEThread::sendI2CPackets(BTI2CPacket* packets, unsigned int num)
{
    pi_warn("Not yet implemented");
}
