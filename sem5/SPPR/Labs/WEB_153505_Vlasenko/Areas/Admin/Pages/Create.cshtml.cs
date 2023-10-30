using Domain.Entities;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.AspNetCore.Mvc.Rendering;
using WEB_153505_Vlasenko.Services.ClothesCategoryService;
using WEB_153505_Vlasenko.Services.ClothesService;

namespace WEB_153505_Vlasenko.Areas.Admin.Pages
{
	//[Authorize(Roles = "admin")]
	public class CreateModel : PageModel
	{
		private readonly IClothesService _clothesService;
		private readonly IClothesCategoryService _clothesCategoryService;

		public CreateModel(IClothesService clothesService, IClothesCategoryService clothesCategoryService)
		{
			_clothesService = clothesService;
			_clothesCategoryService = clothesCategoryService;
		}

		public async Task<IActionResult> OnGet()
		{
			var response = await _clothesCategoryService.GetClothesCategoryListAsync();
			if (!response.Success)
			{
				return NotFound();
			}
			ViewData["CategoryId"] = new SelectList(response.Data, "Id", "Name");
			return Page();
		}

		[BindProperty]
		public Clothes Clothes { get; set; } = default!;

		[BindProperty]
		public IFormFile? Image { get; set; }

		// To protect from overposting attacks, see https://aka.ms/RazorPagesCRUD
		public async Task<IActionResult> OnPostAsync()
		{
			if (!ModelState.IsValid)
			{
				return Page();
			}

			var response = await _clothesService.CreateClothesAsync(Clothes, Image);

			if (!response.Success)
			{
				return Page();
			}

			return RedirectToPage("./Index");
		}
	}
}
