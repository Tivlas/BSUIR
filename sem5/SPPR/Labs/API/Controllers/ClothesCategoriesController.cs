using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using API.Data;
using Domain.Entities;

namespace API.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class ClothesCategoriesController : ControllerBase
    {
        private readonly AppDbContext _context;

        public ClothesCategoriesController(AppDbContext context)
        {
            _context = context;
        }

        // GET: api/ClothesCategories
        [HttpGet]
        public async Task<ActionResult<IEnumerable<ClothesCategory>>> GetClothesCategory()
        {
          if (_context.ClothesCategory == null)
          {
              return NotFound();
          }
            return await _context.ClothesCategory.ToListAsync();
        }

        // GET: api/ClothesCategories/5
        [HttpGet("{id}")]
        public async Task<ActionResult<ClothesCategory>> GetClothesCategory(int id)
        {
          if (_context.ClothesCategory == null)
          {
              return NotFound();
          }
            var clothesCategory = await _context.ClothesCategory.FindAsync(id);

            if (clothesCategory == null)
            {
                return NotFound();
            }

            return clothesCategory;
        }

        // PUT: api/ClothesCategories/5
        // To protect from overposting attacks, see https://go.microsoft.com/fwlink/?linkid=2123754
        [HttpPut("{id}")]
        public async Task<IActionResult> PutClothesCategory(int id, ClothesCategory clothesCategory)
        {
            if (id != clothesCategory.Id)
            {
                return BadRequest();
            }

            _context.Entry(clothesCategory).State = EntityState.Modified;

            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException)
            {
                if (!ClothesCategoryExists(id))
                {
                    return NotFound();
                }
                else
                {
                    throw;
                }
            }

            return NoContent();
        }

        // POST: api/ClothesCategories
        // To protect from overposting attacks, see https://go.microsoft.com/fwlink/?linkid=2123754
        [HttpPost]
        public async Task<ActionResult<ClothesCategory>> PostClothesCategory(ClothesCategory clothesCategory)
        {
          if (_context.ClothesCategory == null)
          {
              return Problem("Entity set 'AppDbContext.ClothesCategory'  is null.");
          }
            _context.ClothesCategory.Add(clothesCategory);
            await _context.SaveChangesAsync();

            return CreatedAtAction("GetClothesCategory", new { id = clothesCategory.Id }, clothesCategory);
        }

        // DELETE: api/ClothesCategories/5
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteClothesCategory(int id)
        {
            if (_context.ClothesCategory == null)
            {
                return NotFound();
            }
            var clothesCategory = await _context.ClothesCategory.FindAsync(id);
            if (clothesCategory == null)
            {
                return NotFound();
            }

            _context.ClothesCategory.Remove(clothesCategory);
            await _context.SaveChangesAsync();

            return NoContent();
        }

        private bool ClothesCategoryExists(int id)
        {
            return (_context.ClothesCategory?.Any(e => e.Id == id)).GetValueOrDefault();
        }
    }
}
