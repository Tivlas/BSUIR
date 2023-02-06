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

        windows.MinimumHeight = 600;
        windows.MinimumWidth = 720;

        windows.MaximumHeight = 800;
        windows.MaximumWidth = 720;

        return windows;
    }
}
