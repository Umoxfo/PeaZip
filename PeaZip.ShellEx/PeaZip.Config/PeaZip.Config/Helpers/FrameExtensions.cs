using Microsoft.UI.Xaml.Controls;

namespace PeaZip.Config.Helpers;

public static class FrameExtensions
{
    public static object? GetPageViewModel(this Frame frame) =>
        frame.CurrentSourcePageType.GetProperty("ViewModel")?.GetValue(frame.Content);
}
