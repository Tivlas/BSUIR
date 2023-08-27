using System.Net.Http;
using System.Text;
using System.Text.Json;
using Domain.Entities;
using Domain.Models;
using WEB_153505_Vlasenko.Services.ClothesCategoryService;

namespace WEB_153505_Vlasenko.Services.ClothesService;

public class ApiClothesService : IClothesService
{
	private readonly HttpClient _httpClient;
	private readonly ILogger<ApiClothesService> _logger;
	private readonly JsonSerializerOptions _jsonSerializerOptions;
	private readonly int _pageSize;

	public ApiClothesService(HttpClient httpClient, IConfiguration configuration, ILogger<ApiClothesService> logger)
	{
		_httpClient = httpClient;
		_logger = logger;
		_jsonSerializerOptions = new JsonSerializerOptions()
		{
			PropertyNamingPolicy = JsonNamingPolicy.CamelCase
		};
		_pageSize = configuration.GetValue<int>("ItemsPerPage");
	}

	public async Task<ResponseData<ListModel<Clothes>>> GetClothesListAsync(string? categoryNormalizedName, int pageNo = 1)
	{
		var urlString = new StringBuilder($"{_httpClient.BaseAddress!.AbsoluteUri}clothes/");
		if (categoryNormalizedName != null)
		{
			urlString.Append($"{categoryNormalizedName}/");
		};
		if (pageNo > 1)
		{
			urlString.Append($"page{pageNo}");
		};
		if (!_pageSize.Equals("3"))
		{
			urlString.Append(QueryString.Create("pageSize", _pageSize.ToString()));
		}

		var response = await _httpClient.GetAsync(new Uri(urlString.ToString()));
		if (response.IsSuccessStatusCode)
		{
			try
			{
				return await response.Content.ReadFromJsonAsync<ResponseData<ListModel<Clothes>>>(_jsonSerializerOptions);
			}
			catch (JsonException ex)
			{
				_logger.LogError($"-----> Ошибка: {ex.Message}");
				return new ResponseData<ListModel<Clothes>>
				{
					Success = false,
					ErrorMessage = $"Ошибка: {ex.Message}"
				};
			}
		}
		_logger.LogError($"-----> Данные не получены от сервера. Error:{response.StatusCode}");
		return new ResponseData<ListModel<Clothes>>()
		{
			Success = false,
			ErrorMessage = $"Данные не получены от сервера. Error:{response.StatusCode}"
		};
	}

	public async Task<ResponseData<Clothes>> GetClothesByIdAsync(int id)
	{

		var urlString = new StringBuilder($"{_httpClient.BaseAddress!.AbsoluteUri}clothes/{id}");
		var response = await _httpClient.GetAsync(new Uri(urlString.ToString()));

		if (response.IsSuccessStatusCode)
		{
			try
			{
				return await response.Content.ReadFromJsonAsync<ResponseData<Clothes>>(_jsonSerializerOptions);
			}
			catch (JsonException ex)
			{
				_logger.LogError($"-----> Ошибка: {ex.Message}");
				return new ResponseData<Clothes>
				{
					Success = false,
					ErrorMessage = $"Ошибка: {ex.Message}"
				};
			}
		}
		_logger.LogError($"-----> Данные не получены от сервера. Error:{response.StatusCode}");
		return new ResponseData<Clothes>()
		{
			Success = false,
			ErrorMessage = $"Данные не получены от сервера. Error:{response.StatusCode}"
		};
	}

	public async Task UpdateClothesAsync(int id, Clothes product, IFormFile? formFile)
	{
		var urlString = new StringBuilder($"{_httpClient.BaseAddress!.AbsoluteUri}clothes/{id}");

		var response = await _httpClient.PutAsync(new Uri(urlString.ToString()),
			new StringContent(JsonSerializer.Serialize(product), Encoding.UTF8, "application/json"));

		if (response.IsSuccessStatusCode)
		{
			if (formFile is not null)
			{
				int clothesId = (await response.Content.ReadFromJsonAsync<ResponseData<Clothes>>(_jsonSerializerOptions)).Data.Id;
				await SaveImageAsync(clothesId, formFile);
			}
		}
		else
		{
			_logger.LogError($"-----> Данные не получены от сервера. Error:{response.StatusCode}");
		}
	}

	public async Task DeleteClothesAsync(int id)
	{
		var uriString = new StringBuilder($"{_httpClient.BaseAddress!.AbsoluteUri}clothes/{id}");

		var response = await _httpClient.DeleteAsync(new Uri(uriString.ToString()));

		if (!response.IsSuccessStatusCode)
		{
			_logger.LogError($"-----> Данные не получены от сервера. Error:{response.StatusCode}");
		}
	}

	public async Task<ResponseData<Clothes>> CreateClothesAsync(Clothes product, IFormFile? formFile)
	{
		var uri = new Uri(_httpClient.BaseAddress!.AbsoluteUri + "Clothes");
		var response = await _httpClient.PostAsJsonAsync(uri, product, _jsonSerializerOptions);

		if (response.IsSuccessStatusCode)
		{
			var data = await response.Content.ReadFromJsonAsync<ResponseData<Clothes>>(_jsonSerializerOptions);
			if (formFile is not null)
			{
				await SaveImageAsync(data.Data.Id, formFile);
			}
			return data;
		}
		_logger.LogError($"-----> Объект не добавлен. Error:{response.StatusCode}");
		return new ResponseData<Clothes>
		{
			Success = false,
			ErrorMessage = $"Объект не добавлен. Error:{response.StatusCode}"
		};
	}

	private async Task SaveImageAsync(int id, IFormFile image)
	{
		var request = new HttpRequestMessage
		{
			Method = HttpMethod.Post,
			RequestUri = new Uri($"{_httpClient.BaseAddress?.AbsoluteUri}Clothes/{id}")
		};
		var content = new MultipartFormDataContent();
		var streamContent = new StreamContent(image.OpenReadStream());
		content.Add(streamContent, "formFile", image.FileName);
		request.Content = content;
		await _httpClient.SendAsync(request);
	}
}
