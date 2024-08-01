using Microsoft.UI.Dispatching;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.Windows.AppLifecycle;

using PeaZip.Config.Activation;
using PeaZip.Config.Contracts.Services;
using PeaZip.Config.Views;

namespace PeaZip.Config.Services;

public class ActivationService : IActivationService
{
    private readonly IActivationHandler<LaunchActivatedEventArgs> defaultHandler;

    private UIElement? _shell;

    public ActivationService(IActivationHandler<LaunchActivatedEventArgs> defaultHandler)
    {
        this.defaultHandler = defaultHandler;
        AppInstance.GetCurrent().Activated += OnActivated;
    }

    public async Task ActivateAsync(object activationArgs)
    {
        // Execute tasks before activation.
        //await InitializeAsync();

        // Set the MainWindow Content.
        if (App.MainWindow.Content is null)
        {
            _shell = App.GetService<ShellPage>();
            App.MainWindow.Content = _shell ?? new Frame();
        }

        // Handle activation via DefaultActivationHandler.
        await defaultHandler.HandleAsync((LaunchActivatedEventArgs)activationArgs);

        // Activate the MainWindow.
        App.MainWindow.Activate();

        // Execute tasks after activation.
        await StartupAsync();
    }

    private void OnActivated(object? sender, AppActivationArguments activationArgs)
    {
        switch (activationArgs.Kind)
        {
            case ExtendedActivationKind.Launch:
                if (activationArgs.Data is Windows.ApplicationModel.Activation.ILaunchActivatedEventArgs args)
                {
                    // Call a launch activation handler
                }
                break;
            case ExtendedActivationKind.File:
            case ExtendedActivationKind.Protocol:
            case ExtendedActivationKind.StartupTask:
            default:
                break;
        }

        App.MainWindow.DispatcherQueue.TryEnqueue(() => App.MainWindow.Activate());
    }

    private Task StartupAsync()
    {
        App.MainWindow.DispatcherQueue.ShutdownStarting += (s, a) =>
        {
            s.TryEnqueue(DispatcherQueuePriority.High, () => AppInstance.GetCurrent().UnregisterKey());
        };

        return Task.CompletedTask;
    }
}
