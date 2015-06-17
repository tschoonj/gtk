#include <gio/gio.h>
#include <stdlib.h>

#define TIMEOUT 1000

enum {
  IDLE,
  SYNCING,
  ERROR
};

typedef struct _CloudProviderClass CloudProviderClass;
typedef struct _CloudProvider CloudProvider;

struct _CloudProviderClass
{
  GObjectClass parent_class;
};

struct _CloudProvider
{
  GObject parent_instance;

  gchar *name;
  gint status;
  GDBusProxy *manager_proxy;
  guint timeout_handler;
};


static GType cloud_provider_get_type (void);
G_DEFINE_TYPE (CloudProvider, cloud_provider, G_TYPE_OBJECT);

static void
cloud_provider_finalize (GObject *object)
{
  CloudProvider *self = (CloudProvider*)object;

  g_free (self->name);

  G_OBJECT_CLASS (cloud_provider_parent_class)->finalize (object);
}

static void
cloud_provider_init (CloudProvider *self)
{
  self->name = "MyCloud";
  self->status = SYNCING;
}

static void
cloud_provider_class_init (CloudProviderClass *class)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);

  gobject_class->finalize = cloud_provider_finalize;
}

static void
cloud_provider_set_status (CloudProvider *self,
                           gint           status)
{
  /* Inform manager that the provider changed */
  self->status = status;
  g_dbus_proxy_call (self->manager_proxy,
                     "CloudProviderChanged",
                     g_variant_new ("()"),
                     G_DBUS_CALL_FLAGS_NONE,
                     -1,
                     NULL,
                     NULL,
                     NULL);
}

/* ---------------------------------------------------------------------------------------------------- */

static GDBusNodeInfo *introspection_data = NULL;

/* Introspection data for the service we are exporting */
static const gchar provider_xml[] =
  "<node>"
  "  <interface name='org.gtk.CloudProvider'>"
  "    <method name='GetName'>"
  "      <arg type='s' name='name' direction='out'/>"
  "    </method>"
  "    <method name='GetStatus'>"
  "      <arg type='i' name='status' direction='out'/>"
  "    </method>"
  "  </interface>"
  "</node>";

static const gchar manager_xml[] =
  "<node>"
  "  <interface name='org.gtk.CloudProviderManager'>"
  "    <method name='CloudProviderChanged'>"
  "    </method>"
  "  </interface>"
  "</node>";

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
  CloudProvider *cloud_provider = user_data;

  g_debug ("Handling dbus call in server\n");
  if (g_strcmp0 (method_name, "GetName") == 0)
    {
      g_dbus_method_invocation_return_value (invocation,
                                             g_variant_new ("(s)", cloud_provider->name));
    }
  else if (g_strcmp0 (method_name, "GetStatus") == 0)
    {
      g_dbus_method_invocation_return_value (invocation,
                                             g_variant_new ("(i)", cloud_provider->status));
    }
}

static const GDBusInterfaceVTable interface_vtable =
{
  handle_method_call,
};

static void
on_bus_acquired (GDBusConnection *connection,
                 const gchar     *name,
                 gpointer         user_data)
{
  CloudProvider *cloud_provider = user_data;
  guint registration_id;

  g_print ("Registering cloud provider server 'MyCloud'\n");
  registration_id = g_dbus_connection_register_object (connection,
                                                       "/org/gtk/CloudProviderServerExample",
                                                       introspection_data->interfaces[0],
                                                       &interface_vtable,
                                                       cloud_provider,
                                                       NULL,  /* user_data_free_func */
                                                       NULL); /* GError** */
  g_assert (registration_id > 0);
}

static void
on_name_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
}

static void
on_name_lost (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
  exit (1);
}

static gboolean
change_provider (gpointer user_data)
{
  CloudProvider *cloud_provider = (CloudProvider *)user_data;
  GRand *rand;
  gint new_status;

  rand = g_rand_new ();
  new_status = g_rand_int_range (rand, IDLE, ERROR + 1);

  cloud_provider_set_status (cloud_provider, new_status);

  return TRUE;
}

static void
on_manager_proxy_created (GObject      *source_object,
                          GAsyncResult *res,
                          gpointer      user_data)
{
  CloudProvider *cloud_provider = user_data;
  GError *error = NULL;

  cloud_provider->manager_proxy = g_dbus_proxy_new_for_bus_finish (res, &error);
  if (error != NULL)
    g_warning ("Error creating proxy for cloud provider manager %s", error->message);
  else
    g_debug ("Manager proxy created for 'MyCloud'\n");

  cloud_provider->timeout_handler = g_timeout_add (TIMEOUT,
                                                   (GSourceFunc) change_provider,
                                                   cloud_provider);
}

int
main (int argc, char *argv[])
{
  GMainLoop *loop;
  CloudProvider *cloud_provider;
  guint owner_id;
  GDBusNodeInfo *proxy_info;
  GDBusInterfaceInfo *interface_info;
  GError *error = NULL;

  /* Export the interface we listen to, so clients can request properties of
   * the cloud provider such as name, status or icon */
  introspection_data = g_dbus_node_info_new_for_xml (provider_xml, NULL);
  g_assert (introspection_data != NULL);

  cloud_provider = g_object_new (cloud_provider_get_type (), NULL);

  owner_id = g_bus_own_name (G_BUS_TYPE_SESSION,
                             "org.gtk.CloudProviderServerExample",
                             G_BUS_NAME_OWNER_FLAGS_NONE,
                             on_bus_acquired,
                             on_name_acquired,
                             on_name_lost,
                             cloud_provider,
                             NULL);

  /* Create CloudProviderManager proxy for exporting cloud provider changes */
  proxy_info = g_dbus_node_info_new_for_xml (manager_xml, &error);
  interface_info = g_dbus_node_info_lookup_interface (proxy_info, "org.gtk.CloudProviderManager");
  g_dbus_proxy_new_for_bus (G_BUS_TYPE_SESSION,
                            G_DBUS_PROXY_FLAGS_NONE,
                            interface_info,
                            "org.gtk.CloudProviderManager",
                            "/org/gtk/CloudProviderManager",
                            "org.gtk.CloudProviderManager",
                            NULL,
                            on_manager_proxy_created,
                            cloud_provider);


  loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (loop);

  g_bus_unown_name (owner_id);

  g_dbus_node_info_unref (introspection_data);

  g_object_unref (cloud_provider);

  return 0;
}

