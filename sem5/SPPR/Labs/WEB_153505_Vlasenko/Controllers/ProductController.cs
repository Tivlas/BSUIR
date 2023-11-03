using Microsoft.AspNetCore.Mvc;
using WEB_153505_Vlasenko.Extensions;
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

	[Route("Clothes")]
	[Route("Clothes/{category?}")]
	public async Task<IActionResult> Index(string? category, int pageNo = 1)
	{
		var productResponse = await _clothesService.GetClothesListAsync(category, pageNo);
		if (!productResponse.Success)
		{
			return NotFound(productResponse.ErrorMessage);
		}
		var allCategories = await _clothesCategoryService.GetClothesCategoryListAsync();
		if (!allCategories.Success)
		{
			return NotFound(allCategories.ErrorMessage);
		}

		ViewData["allCategories"] = allCategories.Data;
		var currentCategory = category == null ? "Все" : allCategories.Data!.FirstOrDefault(c => c.NormalizedName == category)?.Name;
		ViewData["currentCategory"] = currentCategory;
		ViewData["currentPage"] = productResponse.Data!.CurrentPage;
		ViewData["totalPages"] = productResponse.Data.TotalPages;

		if (Request.IsAjaxRequest())
		{
			return PartialView("_ProductCardsAndPagerPartial", new
			{
				Category = category,
				ReturnUrl = Request.Path + Request.QueryString.ToUriComponent(),
				productResponse.Data!.CurrentPage,
				productResponse.Data!.TotalPages,
				Clothes = productResponse.Data!.Items,
				InAdminArea = false
			});
		}

		return View(productResponse.Data!.Items);
	}
}
