namespace PeaZip.ShellEx.Flags;

[Flags]
public enum Browse : uint
{
    None = 0,
    BrowseArchive = 0b1,
    BrowsePath = BrowseArchive << 1,
}
