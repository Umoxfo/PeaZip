using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Navigation;

using PeaZip.Config.ViewModels;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace PeaZip.Config.Views;

/// <summary>
/// An empty page that can be used on its own or navigated to within a Frame.
/// </summary>
public sealed partial class ContextMenuPage : Page
{
    public ContextMenuViewModel ViewModel { get; }

    public ContextMenuPage()
    {
        ViewModel = App.GetService<ContextMenuViewModel>();
        InitializeComponent();
    }

    protected override void OnNavigatedFrom(NavigationEventArgs e)
    {
        ViewModel.OnNavigatedFrom();
        base.OnNavigatedFrom(e);
    }

    protected override void OnNavigatedTo(NavigationEventArgs e)
    {
        ViewModel.OnNavigatedTo(e.Parameter);
        base.OnNavigatedTo(e);
    }
}
