﻿using Domain.Entities;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using WEB_153505_Vlasenko.Extensions;
using WEB_153505_Vlasenko.Services.ClothesService;

namespace WEB_153505_Vlasenko.Areas.Admin.Pages
{
	//[Authorize(Roles = "admin")] пока что роли не работают
	public class IndexModel : PageModel
	{
		private readonly IClothesService _clothesService;

		public IndexModel(IClothesService clothesService)
		{
			_clothesService = clothesService;
		}

		public IList<Clothes> Clothes { get; set; } = default!;

		public int CurrentPage { get; set; }
		public int TotalPages { get; set; }

		public async Task<IActionResult> OnGetAsync(int pageNo = 1)
		{
			var response = await _clothesService.GetClothesListAsync(null, pageNo);

			if (response.Success)
			{
				Clothes = response.Data?.Items!;
				CurrentPage = response.Data.CurrentPage;
				TotalPages = response.Data.TotalPages;
			}

			if (Request.IsAjaxRequest())
			{
				return Partial("_ProductCardsAndPagerPartial", new
				{
					CurrentPage,
					TotalPages,
					Clothes,
					InAdminArea = true,
					ReturnUrl = Request.Path + Request.QueryString.ToUriComponent()
				});
			}

			return Page();
		}
	}
}
