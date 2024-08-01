using System.Diagnostics;

using CommunityToolkit.Mvvm.Messaging;

using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.UI.Xaml;

using PeaZip.Config.Activation;
using PeaZip.Config.Contracts.Services;
using PeaZip.Config.Services;
using PeaZip.Config.ViewModels;
using PeaZip.Config.Views;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace PeaZip.Config;

/// <summary>
/// Provides application-specific behavior to supplement the default Application class.
/// </summary>
public partial class App : Application
{
    public static Window MainWindow { get; } = new MainWindow();

    // The .NET Generic Host provides dependency injection, configuration, logging, and other services.
    // https://docs.microsoft.com/dotnet/core/extensions/generic-host
    // https://docs.microsoft.com/dotnet/core/extensions/dependency-injection
    // https://docs.microsoft.com/dotnet/core/extensions/configuration
    // https://docs.microsoft.com/dotnet/core/extensions/logging
    public IHost Host { get; }

    /// <summary>
    /// Initializes the singleton application object.  This is the first line of authored code
    /// executed, and as such is the logical equivalent of main() or WinMain().
    /// </summary>
    public App()
    {
        InitializeComponent();

        var builder = Microsoft.Extensions.Hosting.Host.CreateApplicationBuilder();

        // Messenger
        builder.Services.AddSingleton<IMessenger, StrongReferenceMessenger>(_ => StrongReferenceMessenger.Default);

        // Default Activation Handler
        builder.Services.AddTransient<IActivationHandler<LaunchActivatedEventArgs>, DefaultActivationHandler>();

        // Services
        builder.Services.AddActivatedSingleton<IActivationService, ActivationService>();

        builder.Services.AddSingleton<IPageService, PageService>();
        builder.Services.AddSingleton<INavigationService, NavigationService>();
        builder.Services.AddTransient<INavigationViewService, NavigationViewService>();

        // Views and ViewModels
        builder.Services.AddSingleton<ContextMenuViewModel>();
        builder.Services.AddTransient<ContextMenuPage>();
        builder.Services.AddTransient<ShellViewModel>();
        builder.Services.AddTransient<ShellPage>();

        Host = builder.Build();
    }

    public static T GetService<T>() where T : class =>
        (Current as App)!.Host.Services.GetRequiredService<T>();

    /// <summary>
    /// Invoked when the application is launched.
    /// </summary>
    /// <param name="args">Details about the launch request and process.</param>
    protected async override void OnLaunched(LaunchActivatedEventArgs args)
    {
        try
        {
            await GetService<IActivationService>().ActivateAsync(args);
        }
        catch (Exception ex)
        {
            Trace.TraceError(ex.ToString());
        }
    }
}
