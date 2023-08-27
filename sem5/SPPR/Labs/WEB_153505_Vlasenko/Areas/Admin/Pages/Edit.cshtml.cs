using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.AspNetCore.Mvc.Rendering;
using Microsoft.EntityFrameworkCore;
using Domain.Entities;
using WEB_153505_Vlasenko.TempDbContext;
using WEB_153505_Vlasenko.Services.ClothesService;

namespace WEB_153505_Vlasenko.Areas.Admin.Pages
{
    public class EditModel : PageModel
    {
        private readonly IClothesService _clothesService;

        public EditModel(IClothesService clothesService)
        {
            _clothesService = clothesService;
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
