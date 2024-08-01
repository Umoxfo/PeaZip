namespace PeaZip.ShellEx.Flags;

[Flags]
public enum ArchiveFormatFeature : uint
{
    None = 0,
    Add7zEncrypt = 0x1,
}
