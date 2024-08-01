using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;

using Microsoft.UI.Xaml.Controls;

using PeaZip.Config.Contracts.Services;
using PeaZip.Config.Helpers;
using PeaZip.Config.Views;

using Windows.ApplicationModel;
using Windows.System;

namespace PeaZip.Config.Services;

public class NavigationViewService(
    INavigationService navigationService,
    IPageService pageService) : INavigationViewService
{
    private const string MainAppId = "PeaZip";
    private static readonly Lazy<Uri> DefaultAppsSettingsUri = new(() =>
        new($"ms-settings:defaultapps?registeredAUMID={AppInfo.Current.PackageFamilyName}!{MainAppId}")
    );

    private NavigationView? _navigationView;

    public IList<object>? MenuItems => _navigationView?.MenuItems;

    [MemberNotNull(nameof(_navigationView))]
    public void Initialize(NavigationView navigationView)
    {
        _navigationView = navigationView;
        _navigationView.ItemInvoked += OnItemInvokedAsync;
    }

    public void UnregisterEvents()
    {
        if (_navigationView is not null)
        {
            _navigationView.ItemInvoked -= OnItemInvokedAsync;
        }
    }

    public NavigationViewItem? GetSelectedItem(Type pageType) =>
        _navigationView is not null
            ? GetSelectedItem(_navigationView.MenuItems, pageType) ?? GetSelectedItem(_navigationView.FooterMenuItems, pageType)
            : null;

    private static NavigationViewItem? GetSelectedItem(IEnumerable<object> menuItems, Type pageType)
    {
        var pageTypeFullName = pageType.FullName!;

        foreach (var item in menuItems.OfType<NavigationViewItem>())
        {
            if (item.GetValue(NavigationHelper.NavigateToProperty) is string pageKey
                && pageKey == pageTypeFullName)
            {
                return item;
            }

            var selectedChild = GetSelectedItem(item.MenuItems, pageType);
            if (selectedChild is not null)
            {
                return selectedChild;
            }
        }

        return null;
    }

    private async void OnItemInvokedAsync(NavigationView sender, NavigationViewItemInvokedEventArgs args)
    {
        try
        {
            var selectedItem = args.InvokedItemContainer as NavigationViewItem;

            if (selectedItem?.GetValue(NavigationHelper.NavigateToProperty) is string pageKey)
            {
                if (pageKey == FileTypeAssociationPage.FullName)
                {
                    var result = Launcher.LaunchUriAsync(DefaultAppsSettingsUri.Value);
                    navigationService.NavigateTo(typeof(FileTypeAssociationPage), transitionInfo: args.RecommendedNavigationTransitionInfo);
                    await result;
                }
                else
                {
                    navigationService.NavigateTo(pageService.GetPageType(pageKey), transitionInfo: args.RecommendedNavigationTransitionInfo);
                }
            }
        }
        catch (Exception ex)
        {
            Trace.TraceError(ex.ToString());
        }
    }
}
