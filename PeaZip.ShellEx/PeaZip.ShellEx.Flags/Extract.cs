namespace PeaZip.ShellEx.Flags;

[Flags]
public enum Extract : uint
{
    None = 0,
    ExtractMain = 0b1,
    ExtractHere = ExtractMain << 1,
    ExtractSmart = ExtractHere << 1,
    ExtractFolder = ExtractSmart << 1,
    ExtractDesktop = ExtractFolder << 1,
    ExtractDocument = ExtractDesktop << 1,
    ExtractDownload = ExtractDocument << 1,
    ExtractTest = ExtractDownload << 1,
}
