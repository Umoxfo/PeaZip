namespace PeaZip.ShellEx.Flags;

[Flags]
public enum ArchiveFeature : uint
{
    None = 0,
    AddZipMail = 0b1,
    Split = AddZipMail << 1,
    Convert = Split << 1,
}
