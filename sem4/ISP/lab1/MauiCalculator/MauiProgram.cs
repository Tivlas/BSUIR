using MauiCalculator.Lab3;
using MauiCalculator.Lab3.Services;
namespace MauiCalculator;

public static class MauiProgram
{
    public static MauiApp CreateMauiApp()
    {
        var builder = MauiApp.CreateBuilder();
        builder
            .UseMauiApp<App>()
            .ConfigureFonts(fonts =>
            {
                fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular");
                fonts.AddFont("OpenSans-Semibold.ttf", "OpenSansSemibold");
            });
        builder.Services.AddTransient<IDbService, SQLiteService>();
        builder.Services.AddSingleton<lab3>();

        return builder.Build();
    }
}
