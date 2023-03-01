using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Serialization;
using System.Threading.Tasks;
using System.Net.Http.Json;
using MauiCalculator.Lab4.Entities;

namespace MauiCalculator.Lab4.Services
{
    public class CurrencyService : ICurrencyService
    {
        private HttpClient _client;

        public CurrencyService(HttpClient client)
        {
            _client = client;
        }

        public async Task<IEnumerable<Currency>> GetCurrenciesAsync()
        {
            var result =  _client.Send(new HttpRequestMessage(HttpMethod.Get, _client.BaseAddress));
            var response = await HttpContentJsonExtensions.ReadFromJsonAsync<IEnumerable<Currency>>(result.Content);
            return response.Where(currency => currency.Cur_DateEnd > DateTime.Today);
        }
    }
}
