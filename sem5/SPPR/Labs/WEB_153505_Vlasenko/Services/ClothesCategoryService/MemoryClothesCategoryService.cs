using Domain.Entities;
using Domain.Models;

namespace WEB_153505_Vlasenko.Services.ClothesCategoryService;

public class MemoryClothesCategoryService : IClothesCategoryService
{
	public Task<ResponseData<List<ClothesCategory>>> GetClothesCategoryListAsync()
	{
		var categories = new List<ClothesCategory>
		{
			new ClothesCategory {Id=1, Name="Обувь", NormalizedName="shoes"},
			new ClothesCategory {Id=2, Name="Куртки", NormalizedName="jackets"},
		};
		var result = new ResponseData<List<ClothesCategory>>();
		result.Data = categories;
		return Task.FromResult(result);
	}
}
