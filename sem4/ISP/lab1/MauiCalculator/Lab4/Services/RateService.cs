
using MauiCalculator.Lab4.Entities;
using System.Net.Http.Json;

namespace MauiCalculator.Lab4.Services;

public class RateService : IRateService
{
    private HttpClient _client;

    public RateService(HttpClient client)
    {
        _client = client;
    }

    public async Task<Rate> GetRate(DateTime date, Currency currency)
    {
        var result = _client.Send(new HttpRequestMessage(HttpMethod.Get, $"{currency.Cur_ID}?ondate={date:yyyy-M-d}"));
        var response = await HttpContentJsonExtensions.ReadFromJsonAsync<Rate>(result.Content);
        return response!;
    }
}
