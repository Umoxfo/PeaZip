using System.Diagnostics.CodeAnalysis;

using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Media.Animation;
using Microsoft.UI.Xaml.Navigation;

using PeaZip.Config.Contracts.Services;
using PeaZip.Config.Contracts.ViewModels;
using PeaZip.Config.Helpers;

namespace PeaZip.Config.Services;

// For more information on navigation between pages see
// https://github.com/microsoft/TemplateStudio/blob/main/docs/WinUI/navigation.md
public class NavigationService() : INavigationService
{
    private object? _lastParameterUsed;

    private Frame? _frame;
    public Frame Frame
    {
        get
        {
            if (_frame is null)
            {
                _frame = App.MainWindow.Content as Frame ?? new Frame();
                _frame.Navigated += OnNavigated;
            }

            return _frame;
        }

        set
        {
            if (_frame is not null)
            {
                _frame.Navigated -= OnNavigated;
            }

            _frame = value;
            _frame.Navigated += OnNavigated;
        }
    }

    [MemberNotNullWhen(true, nameof(Frame), nameof(_frame))]
    public bool CanGoBack => Frame is not null && Frame.CanGoBack;

    public bool NavigateTo(Type pageType, object? parameter = null, NavigationTransitionInfo? transitionInfo = null, bool clearNavigation = false)
    {
        if (_frame is not null && (_frame.CurrentSourcePageType != pageType || parameter is not null && !parameter.Equals(_lastParameterUsed)))
        {
            _frame.Tag = clearNavigation;

            if (_frame.CurrentSourcePageType is not null && _frame.GetPageViewModel() is INavigationAware navigationAware)
            {
                navigationAware.OnNavigatedFrom();
            }

            var navigated = transitionInfo is not null
               ? _frame.Navigate(pageType, parameter, transitionInfo)
               : _frame.Navigate(pageType, parameter);
            if (navigated)
            {
                _lastParameterUsed = parameter;
            }

            return navigated;
        }

        return false;
    }

    private void OnNavigated(object sender, NavigationEventArgs e)
    {
        if (sender is Frame frame)
        {
            var clearNavigation = (bool)frame.Tag;
            if (clearNavigation)
            {
                frame.BackStack.Clear();
            }

            //if (frame.GetPageViewModel() is INavigationAware navigationAware)
            //{
            //    navigationAware.OnNavigatedTo(e.Parameter);
            //}
        }
    }
}
