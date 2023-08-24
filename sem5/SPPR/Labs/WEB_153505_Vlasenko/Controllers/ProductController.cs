using Domain.Entities;
using Microsoft.AspNetCore.Mvc;
using WEB_153505_Vlasenko.Services.ClothesCategoryService;
using WEB_153505_Vlasenko.Services.ClothesService;

namespace WEB_153505_Vlasenko.Controllers;
public class ProductController : Controller
{
	private readonly IClothesService _clothesService;
	private readonly IClothesCategoryService _clothesCategoryService;

	public ProductController(IClothesService clothesService, IClothesCategoryService clothesCategoryService)
	{
		_clothesService = clothesService;
		_clothesCategoryService = clothesCategoryService;
	}

	public async Task<IActionResult> Index(string? category, string? currentCategory, int pageNo = 1)
	{
		ViewData["currentcategory"] = currentCategory;
		var productResponse = await _clothesService.GetClothesListAsync(category, pageNo);
		if (!productResponse.Success)
		{
			return NotFound(productResponse.ErrorMessage);
		}
		var allCategories = await _clothesCategoryService.GetClothesCategoryListAsync();
		return View((productResponse.Data!.Items, allCategories.Data,
			productResponse.Data.CurrentPage, productResponse.Data.TotalPages));
	}
}
