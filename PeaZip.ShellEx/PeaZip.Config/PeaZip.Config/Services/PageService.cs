using System.Collections.Frozen;

using PeaZip.Config.Contracts.Services;
using PeaZip.Config.Views;

namespace PeaZip.Config.Services;

public class PageService : IPageService
{
    private readonly FrozenDictionary<string, Type> _pages = new Dictionary<string, Type>(StringComparer.Ordinal)
    {
        { typeof(ContextMenuPage).FullName!, typeof(ContextMenuPage) }
    }
    .ToFrozenDictionary();

    public Type GetPageType(string key)
    {
        if (!_pages.TryGetValue(key, out var pageType))
        {
            throw new ArgumentException($"Page not found: {key}. Did you forget to call PageService.Configure?");
        }

        return pageType;
    }
}
