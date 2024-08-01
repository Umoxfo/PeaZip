using CommunityToolkit.Mvvm.Messaging;

using Microsoft.UI.Xaml;

using PeaZip.Config.Contracts.Services;
using PeaZip.Config.Models;
using PeaZip.Config.Views;

namespace PeaZip.Config.Activation;

public sealed class DefaultActivationHandler(
    INavigationService navigationService,
    IMessenger messenger) : IActivationHandler<LaunchActivatedEventArgs>
{
    public bool CanHandle(LaunchActivatedEventArgs args) =>
        navigationService.Frame?.Content is null;

    public Task HandleAsync(LaunchActivatedEventArgs args)
    {
        var navigatePageType = typeof(ContextMenuPage);
        navigationService.NavigateTo(navigatePageType, args.Arguments);
        messenger.Send(new ActivationMessage(navigatePageType));

        return Task.CompletedTask;
    }
}
