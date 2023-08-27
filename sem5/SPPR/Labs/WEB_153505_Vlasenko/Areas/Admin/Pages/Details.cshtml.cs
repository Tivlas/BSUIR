﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using Domain.Entities;
using WEB_153505_Vlasenko.TempDbContext;
using WEB_153505_Vlasenko.Services.ClothesService;

namespace WEB_153505_Vlasenko.Areas.Admin.Pages
{
    public class DetailsModel : PageModel
    {
        private readonly IClothesService _clothesService;

        public DetailsModel(IClothesService clothesService)
        {
            _clothesService = clothesService;
        }

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
    }
}