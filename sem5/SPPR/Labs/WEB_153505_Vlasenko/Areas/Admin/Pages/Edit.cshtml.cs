using Domain.Entities;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.AspNetCore.Mvc.Rendering;
using WEB_153505_Vlasenko.Services.ClothesCategoryService;
using WEB_153505_Vlasenko.Services.ClothesService;

namespace WEB_153505_Vlasenko.Areas.Admin.Pages
{
	//[Authorize(Roles = "admin")]
	public class EditModel : PageModel
    {
        private readonly IClothesService _clothesService;
        private readonly IClothesCategoryService _clothesCategoryService;

        public EditModel(IClothesService clothesService, IClothesCategoryService clothesCategoryService)
        {
            _clothesService = clothesService;
            _clothesCategoryService = clothesCategoryService;
        }

        [BindProperty]
        public Clothes Clothes { get; set; } = default!;

        [BindProperty]
        public IFormFile? Image { get; set; }

        public async Task<IActionResult> OnGetAsync(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var response = await _clothesService.GetClothesByIdAsync(id.Value);

            if (!response.Success)
            {
                return NotFound();
            }

            var responseCtg = await _clothesCategoryService.GetClothesCategoryListAsync();
            if (!response.Success)
            {
                return NotFound();
            }
            ViewData["CategoryId"] = new SelectList(responseCtg.Data, "Id", "Name");

            Clothes = response.Data!;

            return Page();
        }

        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see https://aka.ms/RazorPagesCRUD.
        public async Task<IActionResult> OnPostAsync()
        {
            if (!ModelState.IsValid)
            {
                return Page();
            }


            try
            {
                await _clothesService.UpdateClothesAsync(Clothes.Id, Clothes, Image);
            }
            catch (Exception)
            {
                if (!await ClothesExists(Clothes.Id))
                {
                    return NotFound();
                }
                else
                {
                    throw;
                }
            }

            return RedirectToPage("./Index");
        }

        private async Task<bool> ClothesExists(int id)
        {
            var response = await _clothesService.GetClothesByIdAsync(id);
            return response.Success;
        }
    }
}
