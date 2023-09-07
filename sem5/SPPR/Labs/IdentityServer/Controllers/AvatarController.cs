using IdentityServer.Models;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.StaticFiles;

namespace IdentityServer.Controllers;
[Route("[controller]")]
[ApiController]
[Authorize]
public class AvatarController : ControllerBase
{
	private readonly IWebHostEnvironment _webHostEnvironment;
	private readonly UserManager<ApplicationUser> _userManager;

	public AvatarController(IWebHostEnvironment webHostEnvironment, UserManager<ApplicationUser> userManager)
	{
		_webHostEnvironment = webHostEnvironment;
		_userManager = userManager;
	}

	[HttpGet]
	public async Task<IActionResult> GetAvatar()
	{
		ApplicationUser user = await _userManager.GetUserAsync(User);
		FileExtensionContentTypeProvider contentTypeProvider = new();
		var contentType = string.Empty;
		if (user != null)
		{
			string imagePath = Path.Combine(_webHostEnvironment.ContentRootPath, "images", $"{user.Id}.png");
			contentTypeProvider.TryGetContentType(imagePath, out contentType);
			if (System.IO.File.Exists(imagePath))
			{
				return PhysicalFile(imagePath, contentType);
			}
		}

		string defaultFilePath = Path.Combine(_webHostEnvironment.ContentRootPath, "images", "default.jpg");
		contentTypeProvider.TryGetContentType(defaultFilePath, out contentType);

		return PhysicalFile(defaultFilePath, contentType);
	}
}
