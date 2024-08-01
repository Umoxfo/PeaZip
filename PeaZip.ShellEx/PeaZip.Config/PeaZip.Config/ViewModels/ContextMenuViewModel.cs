using CommunityToolkit.Mvvm.ComponentModel;

using PeaZip.Config.Contracts.ViewModels;
using PeaZip.ShellEx.Flags;

using Windows.Storage;

namespace PeaZip.Config.ViewModels;

public partial class ContextMenuViewModel : ObservableObject, INavigationAware
{
    private static readonly ApplicationDataContainer localSettings = ApplicationData.Current.LocalSettings;

    public ContextMenuViewModel() => App.MainWindow.Closed += (s, e) => StoreContextMenuConfig();

    #region Extract
    private Extract extract = Extract.None;

    public bool ExtractMain
    {
        get => extract.HasFlag(Extract.ExtractMain);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.ExtractMain);
            extract ^= Extract.ExtractMain;
            OnPropertyChanged(PropertyChangedArgs.ExtractMain);
        }
    }

    public bool ExtractHere
    {
        get => extract.HasFlag(Extract.ExtractHere);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.ExtractHere);
            extract ^= Extract.ExtractHere;
            OnPropertyChanged(PropertyChangedArgs.ExtractHere);
        }
    }

    public bool ExtractSmart
    {
        get => extract.HasFlag(Extract.ExtractSmart);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.ExtractSmart);
            extract ^= Extract.ExtractSmart;
            OnPropertyChanged(PropertyChangedArgs.ExtractSmart);
        }
    }

    public bool ExtractFolder
    {
        get => extract.HasFlag(Extract.ExtractFolder);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.ExtractFolder);
            extract ^= Extract.ExtractFolder;
            OnPropertyChanged(PropertyChangedArgs.ExtractFolder);
        }
    }

    public bool ExtractDesktop
    {
        get => extract.HasFlag(Extract.ExtractDesktop);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.ExtractDesktop);
            extract ^= Extract.ExtractDesktop;
            OnPropertyChanged(PropertyChangedArgs.ExtractDesktop);
        }
    }

    public bool ExtractDocument
    {
        get => extract.HasFlag(Extract.ExtractDocument);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.ExtractDocument);
            extract ^= Extract.ExtractDocument;
            OnPropertyChanged(PropertyChangedArgs.ExtractDocument);
        }
    }

    public bool ExtractDownload
    {
        get => extract.HasFlag(Extract.ExtractDownload);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.ExtractDownload);
            extract ^= Extract.ExtractDownload;
            OnPropertyChanged(PropertyChangedArgs.ExtractDownload);
        }
    }

    public bool ExtractTest
    {
        get => extract.HasFlag(Extract.ExtractTest);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.ExtractTest);
            extract ^= Extract.ExtractTest;
            OnPropertyChanged(PropertyChangedArgs.ExtractTest);
        }
    }
    #endregion

    #region Browse
    private Browse browse = Browse.None;

    public bool BrowseArchive
    {
        get => browse.HasFlag(Browse.BrowseArchive);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.BrowseArchive);
            browse ^= Browse.BrowseArchive;
            OnPropertyChanged(PropertyChangedArgs.BrowseArchive);
        }
    }

    public bool BrowsePath
    {
        get => browse.HasFlag(Browse.BrowsePath);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.BrowsePath);
            browse ^= Browse.BrowsePath;
            OnPropertyChanged(PropertyChangedArgs.BrowsePath);
        }
    }
    #endregion

    #region ArchiveFormat
    private ArchiveFormat archiveFormat = ArchiveFormat.None;

    public bool AddArchive
    {
        get => archiveFormat.HasFlag(ArchiveFormat.AddArchive);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.AddArchive);
            archiveFormat ^= ArchiveFormat.AddArchive;
            OnPropertyChanged(PropertyChangedArgs.AddArchive);
        }
    }

    public bool AddSeparateArchive
    {
        get => archiveFormat.HasFlag(ArchiveFormat.AddSeparateArchive);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.AddSeparateArchive);
            archiveFormat ^= ArchiveFormat.AddSeparateArchive;
            OnPropertyChanged(PropertyChangedArgs.AddSeparateArchive);
        }
    }

    public bool AddSfx
    {
        get => archiveFormat.HasFlag(ArchiveFormat.AddSfx);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.AddSfx);
            archiveFormat ^= ArchiveFormat.AddSfx;
            OnPropertyChanged(PropertyChangedArgs.AddSfx);
        }
    }

    public bool Add7z
    {
        get => archiveFormat.HasFlag(ArchiveFormat.Add7z);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.Add7z);
            archiveFormat ^= ArchiveFormat.Add7z;
            OnPropertyChanged(PropertyChangedArgs.Add7z);
        }
    }

    public bool AddBrotli
    {
        get => archiveFormat.HasFlag(ArchiveFormat.AddBrotli);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.AddBrotli);
            archiveFormat ^= ArchiveFormat.AddBrotli;
            OnPropertyChanged(PropertyChangedArgs.AddBrotli);
        }
    }

    public bool AddBZip2
    {
        get => archiveFormat.HasFlag(ArchiveFormat.AddBZip2);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.AddBZip2);
            archiveFormat ^= ArchiveFormat.AddBZip2;
            OnPropertyChanged(PropertyChangedArgs.AddBZip2);
        }
    }

    public bool AddGz
    {
        get => archiveFormat.HasFlag(ArchiveFormat.AddGz);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.AddGz);
            archiveFormat ^= ArchiveFormat.AddGz;
            OnPropertyChanged(PropertyChangedArgs.AddGz);
        }
    }

    public bool AddPea
    {
        get => archiveFormat.HasFlag(ArchiveFormat.AddPea);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.AddPea);
            archiveFormat ^= ArchiveFormat.AddPea;
            OnPropertyChanged(PropertyChangedArgs.AddPea);
        }
    }

    public bool AddTar
    {
        get => archiveFormat.HasFlag(ArchiveFormat.AddTar);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.AddTar);
            archiveFormat ^= ArchiveFormat.AddTar;
            OnPropertyChanged(PropertyChangedArgs.AddTar);
        }
    }

    public bool AddWim
    {
        get => archiveFormat.HasFlag(ArchiveFormat.AddWim);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.AddWim);
            archiveFormat ^= ArchiveFormat.AddWim;
            OnPropertyChanged(PropertyChangedArgs.AddWim);
        }
    }

    public bool AddXz
    {
        get => archiveFormat.HasFlag(ArchiveFormat.AddXz);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.AddXz);
            archiveFormat ^= ArchiveFormat.AddXz;
            OnPropertyChanged(PropertyChangedArgs.AddXz);
        }
    }

    public bool AddZip
    {
        get => archiveFormat.HasFlag(ArchiveFormat.AddZip);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.AddZip);
            archiveFormat ^= ArchiveFormat.AddZip;
            OnPropertyChanged(PropertyChangedArgs.AddZip);
        }
    }

    public bool AddZqap
    {
        get => archiveFormat.HasFlag(ArchiveFormat.AddZqap);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.AddZqap);
            archiveFormat ^= ArchiveFormat.AddZqap;
            OnPropertyChanged(PropertyChangedArgs.AddZqap);
        }
    }

    public bool AddZstd
    {
        get => archiveFormat.HasFlag(ArchiveFormat.AddZstd);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.AddZstd);
            archiveFormat ^= ArchiveFormat.AddZstd;
            OnPropertyChanged(PropertyChangedArgs.AddZstd);
        }
    }
    #endregion

    #region ArchiveFormatFeature
    private ArchiveFormatFeature archiveFormatFeature = ArchiveFormatFeature.None;

    public bool Add7zEncrypt
    {
        get => archiveFormatFeature.HasFlag(ArchiveFormatFeature.Add7zEncrypt);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.Add7zEncrypt);
            archiveFormatFeature ^= ArchiveFormatFeature.Add7zEncrypt;
            OnPropertyChanged(PropertyChangedArgs.Add7zEncrypt);
        }
    }
    #endregion

    #region ArchiveFeature
    private ArchiveFeature archiveFeature = ArchiveFeature.None;

    public bool AddZipMail
    {
        get => archiveFeature.HasFlag(ArchiveFeature.AddZipMail);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.AddZipMail);
            archiveFeature ^= ArchiveFeature.AddZipMail;
            OnPropertyChanged(PropertyChangedArgs.AddZipMail);
        }
    }

    public bool Split
    {
        get => archiveFeature.HasFlag(ArchiveFeature.Split);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.Split);
            archiveFeature ^= ArchiveFeature.Split;
            OnPropertyChanged(PropertyChangedArgs.Split);
        }
    }

    public bool Convert
    {
        get => archiveFeature.HasFlag(ArchiveFeature.Convert);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.Convert);
            archiveFeature ^= ArchiveFeature.Convert;
            OnPropertyChanged(PropertyChangedArgs.Convert);
        }
    }
    #endregion

    #region Utils
    private Utils utils = Utils.None;

    public bool Analyze
    {
        get => utils.HasFlag(Utils.Analyze);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.Analyze);
            utils ^= Utils.Analyze;
            OnPropertyChanged(PropertyChangedArgs.Analyze);
        }
    }

    public bool Wipe
    {
        get => utils.HasFlag(Utils.Wipe);
        set
        {
            OnPropertyChanging(PropertyChangingArgs.Wipe);
            utils ^= Utils.Wipe;
            OnPropertyChanged(PropertyChangedArgs.Wipe);
        }
    }
    #endregion

    public async void OnNavigatedFrom() => await Task.Run(StoreContextMenuConfig).ConfigureAwait(false);

    public void OnNavigatedTo(object parameter) => LoadContextMenuConfig();
}
