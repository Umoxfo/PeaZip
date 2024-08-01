namespace PeaZip.Config.Activation;

public interface IActivationHandler<T> where T : class
{
    bool CanHandle(T args);

    Task HandleAsync(T args);
}
