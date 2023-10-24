using Domain.Models;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using WEB_153505_Vlasenko.Extensions;
using WEB_153505_Vlasenko.Services.ClothesService;

namespace WEB_153505_Vlasenko.Controllers;
public class CartController : Controller
{
	private readonly IClothesService _clothesService;
	private readonly Cart _sessionCart;

	public CartController(IClothesService clothesService, Cart sessionCart)
	{
		_clothesService = clothesService;
		_sessionCart = sessionCart;
	}

	public IActionResult Index()
	{
		return View(_sessionCart);
	}

	[Authorize]
	[Route("[controller]/add/{id:int}")]
	public async Task<ActionResult> Add(int id, string returnUrl)
	{
		var data = await _clothesService.GetClothesByIdAsync(id);
		if (data.Success)
		{
			_sessionCart.Add(data.Data!);
		}
		return Redirect(returnUrl);
	}

	[Authorize]
	[Route("[controller]/remove/{id:int}")]
	public async Task<ActionResult> Remove(int id, string returnUrl)
	{
		var data = await _clothesService.GetClothesByIdAsync(id);
		if (data.Success)
		{
			_sessionCart.Remove(data.Data!.Id);
		}
		return Redirect(returnUrl);
	}

}
