using Domain.Entities;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using WEB_153505_Vlasenko.Services.ClothesService;

namespace WEB_153505_Vlasenko.Areas.Admin.Pages
{
	//[Authorize(Roles = "admin")]
	public class DeleteModel : PageModel
    {
        private readonly IClothesService _clothesService;

        public DeleteModel(IClothesService clothesService)
        {
            _clothesService = clothesService;
        }

        [BindProperty]
        public Clothes Clothes { get; set; } = default!;

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

            Clothes = response.Data!;

            return Page();
        }


        public async Task<IActionResult> OnPostAsync(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            try
            {
                await _clothesService.DeleteClothesAsync(id.Value);
            }
            catch (Exception e)
            {
                return NotFound(e.Message);
            }

            return RedirectToPage("./Index");
        }
    }
}
