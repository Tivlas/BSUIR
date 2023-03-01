using MauiCalculator.Lab3.Services;
using MauiCalculator.Lab4;
using MauiCalculator.Lab4.Services;
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

        builder.Services.AddHttpClient<IRateService, RateService>(opt => opt.BaseAddress = new Uri("https://www.nbrb.by/api/exrates/rates/"));
        builder.Services.AddSingleton<CurrencyConverter>();
        builder.Services.AddHttpClient<ICurrencyService, CurrencyService>(opt => opt.BaseAddress = new Uri("https://www.nbrb.by/api/exrates/currencies"));

        return builder.Build();
    }
}
