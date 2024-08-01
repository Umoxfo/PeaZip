using System.Diagnostics.CodeAnalysis;

using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Messaging;

using Microsoft.UI.Xaml;

using PeaZip.Config.Contracts.Services;
using PeaZip.Config.Models;

namespace PeaZip.Config.ViewModels;

public partial class ShellViewModel : ObservableRecipient
{
    [ObservableProperty]
    private object? selected;

    public ShellViewModel(
        INavigationService navigationService,
        INavigationViewService navigationViewService,
        IMessenger messenger) : base(messenger)
    {
        NavigationService = navigationService;
        NavigationViewService = navigationViewService;
        CallOnActivated();

        App.MainWindow.Closed += (s, a) => OnDeactivated();
    }

    public INavigationService NavigationService { get; }

    public INavigationViewService NavigationViewService { get; }

    protected override void OnActivated()
    {
        Messenger.Register<ShellViewModel, ActivationMessage>(this, static (r, m) =>
            r.Selected = r.NavigationViewService.GetSelectedItem(m.Type));
    }

    internal Thickness SetTitleBarMargin(double offset) => new(offset, 0, 0, 0);

    [UnconditionalSuppressMessage("Trimming", "IL2026:RequiresUnreferencedCodeAttribute",
        Justification = "Override OnActivated() and register the required messages individually.")]
    private void CallOnActivated() => OnActivated();
}
