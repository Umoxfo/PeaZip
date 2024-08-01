using System.Runtime.InteropServices;

using Microsoft.UI.Dispatching;
using Microsoft.UI.Xaml;
using Microsoft.Windows.AppLifecycle;

using WinRT;

namespace PeaZip.Config;

/// <summary>
/// Program class
/// </summary>
public static class Program
{
    [DllImport("Microsoft.ui.xaml.dll")]
    [DefaultDllImportSearchPaths(DllImportSearchPath.SafeDirectories)]
    private static extern void XamlCheckProcessRequirements();

    [STAThread]
    static async Task Main(string[] args)
    {
        XamlCheckProcessRequirements();

        ComWrappersSupport.InitializeComWrappers();
        if (await DecideRedirectionAsync())
        {
            Application.Start((p) =>
            {
                var context = new DispatcherQueueSynchronizationContext(DispatcherQueue.GetForCurrentThread());
                SynchronizationContext.SetSynchronizationContext(context);
                _ = new App();
            });
        }
    }

    /// <summary>
    /// Determines whether to redirect the current activation request to another app instance or launch a new app instance.
    /// </summary>
    /// <returns><see langword="true"/> if launch a new app instance, otherwise <see langword="false"/></returns>
    /// <remarks>
    /// The <see cref="DecideRedirectionAsync"/> determines if the instance is existed by registering a key representing the app instance.
    /// Based on the result of key registration, it infers if there is a current app instancing running;
    /// therefore, whether or not to redirect or allow the app to launch for the first time.
    /// </remarks>
    private static async Task<bool> DecideRedirectionAsync()
    {
        var keyInstance = AppInstance.FindOrRegisterForKey("main");

        bool isCurrent = keyInstance.IsCurrent;
        if (!isCurrent)
        {
            // Do the redirection on another thread, and use a non-blocking
            // wait method to wait for the redirection to complete.
            await keyInstance.RedirectActivationToAsync(AppInstance.GetCurrent().GetActivatedEventArgs());
        }

        return isCurrent;
    }
}
