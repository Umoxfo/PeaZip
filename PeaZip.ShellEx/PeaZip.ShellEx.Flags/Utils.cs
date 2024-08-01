namespace PeaZip.ShellEx.Flags;

[Flags]
public enum Utils : uint
{
    None = 0,
    Analyze = 0b1,
    Wipe = Analyze << 1,
}
