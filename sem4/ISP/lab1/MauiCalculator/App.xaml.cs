namespace MauiCalculator;

public partial class App : Application
{
    public App()
    {
        InitializeComponent();

        MainPage = new AppShell();
    }

    protected override Window CreateWindow(IActivationState activationState)
    {
        var windows = base.CreateWindow(activationState);

        windows.MinimumHeight = 550;
        windows.MinimumWidth = 650;

        return windows;
    }
}
