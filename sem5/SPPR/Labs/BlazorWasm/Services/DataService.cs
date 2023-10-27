using System.Net.Http.Headers;
using System.Net.Http;
using System.Text.Json;
using System.Text;
using Domain.Entities;
using Domain.Models;
using System.Net.Http.Json;
using Microsoft.AspNetCore.Http;

namespace BlazorWasm.Services;

public class DataService : IDataService
{
	private readonly HttpClient _httpClient;
	private readonly int _pageSize = 3;
	private readonly JsonSerializerOptions _jsonSerializerOptions;

	public DataService(HttpClient httpClient, IConfiguration configuration)
	{
		_httpClient = httpClient;
		_pageSize = configuration.GetSection("PageSize").Get<int>();
		_jsonSerializerOptions = new JsonSerializerOptions()
		{
			PropertyNamingPolicy = JsonNamingPolicy.CamelCase
		};
	}

	public List<ClothesCategory>? Categories { get; set; }
	public List<Clothes>? ClothesList { get; set; }
	public bool Success { get; set; } = true;
	public string? ErrorMessage { get; set; }
	public int TotalPages { get; set; }
	public int CurrentPage { get; set; }

	public async Task GetClothesListAsync(string? categoryNormalizedName, int pageNo = 1)
	{
		var urlString = new StringBuilder($"{_httpClient.BaseAddress!.AbsoluteUri}api/Clothes/");
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
				var responseData = await response.Content.ReadFromJsonAsync<ResponseData<ListModel<Clothes>>>(_jsonSerializerOptions);
				ClothesList = responseData?.Data?.Items;
				TotalPages = responseData?.Data?.TotalPages ?? 0;
				CurrentPage = responseData?.Data?.CurrentPage ?? 0;
			}
			catch (JsonException ex)
			{
				Success = false;
				ErrorMessage = $"Ошибка: {ex.Message}";
			}
		}
		else
		{
			Success = false;
			ErrorMessage = $"Данные не получены от сервера. Error:{response.StatusCode}";
		}
	}

	public async Task<Clothes?> GetClothesByIdAsync(int id)
	{
		var urlString = new StringBuilder($"{_httpClient.BaseAddress!.AbsoluteUri}api/clothes/{id}");
		var response = await _httpClient.GetAsync(new Uri(urlString.ToString()));

		if (response.IsSuccessStatusCode)
		{
			try
			{
				return (await response.Content.ReadFromJsonAsync<ResponseData<Clothes>>(_jsonSerializerOptions))?.Data;
			}
			catch (JsonException ex)
			{
				Success = false;
				ErrorMessage = $"Ошибка: {ex.Message}";
				return null;
			}
		}
		Success = false;
		ErrorMessage = $"Данные не получены от сервера. Error:{response.StatusCode}";
		return null;
	}

	public async Task GetCategoryListAsync()
	{
		var urlString = new StringBuilder($"{_httpClient.BaseAddress?.AbsoluteUri}api/clothesCategories/");
		var response = await _httpClient.GetAsync(new Uri(urlString.ToString()));
		if (response.IsSuccessStatusCode)
		{
			try
			{
				var responseData = await response.Content.ReadFromJsonAsync<ResponseData<List<ClothesCategory>>>(_jsonSerializerOptions);
				Categories = responseData?.Data;
			}
			catch (JsonException ex)
			{
				Success = false;
				ErrorMessage = $"Ошибка: {ex.Message}";
			}
		}
		else
		{
			Success = false;
			ErrorMessage = $"Данные не получены от сервера. Error:{response.StatusCode}";
		}
	}
}
