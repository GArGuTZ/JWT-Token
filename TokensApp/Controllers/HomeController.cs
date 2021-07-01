using Microsoft.AspNetCore.Mvc;
using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.AspNetCore.Mvc;
using Microsoft.IdentityModel.Tokens;
using System.IdentityModel.Tokens.Jwt;
using System.Security.Claims;
using TokenApp.Models;

namespace TokenApp.Controllers
{
    public class HomeController : Controller
    {
        [HttpPost("/token")]
        public IActionResult Token(string username, string password)
        {
            var identity = GetIdentity(username, password);
            if (identity == null)
            {
                return BadRequest(new { errorText = "Invalid username or password." });
            }

            var now = DateTime.UtcNow;
            var jwt = new JwtSecurityToken(
                    issuer: AuthOptions.ISSUER,
                    audience: AuthOptions.AUDIENCE,
                    notBefore: now,
                    claims: identity.Claims,
                    expires: now.Add(TimeSpan.FromMinutes(AuthOptions.LIFETIME)),
                    signingCredentials: new SigningCredentials(AuthOptions.GetSymmetricSecurityKey(), SecurityAlgorithms.HmacSha256));
            var encodedJwt = new JwtSecurityTokenHandler().WriteToken(jwt);

            var response = new
            {
                access_token = encodedJwt,
                username = identity.Name
            };

            return Json(response);
        }
        private ClaimsIdentity GetIdentity(string username, string password)
        {
            Models.ApplicationContext db = new Models.ApplicationContext();
            User user = db.Users.FirstOrDefault(x => x.Login == username && x.Password == password);
            if (user != null)
            {
                var claims = new List<Claim>
                {
                    new Claim(ClaimsIdentity.DefaultNameClaimType, user.Login)
                };
                ClaimsIdentity claimsIdentity =
                new ClaimsIdentity(claims, "Token", ClaimsIdentity.DefaultNameClaimType,
                    ClaimsIdentity.DefaultRoleClaimType);
                return claimsIdentity;
            }
            return null;
        }
        [HttpGet("/getHistory/{count}")]
        public IActionResult GetHistory(int count)
        {
            Models.ApplicationContext db = new Models.ApplicationContext();
            Console.WriteLine(count);
            var selectedMessages = (from m in db.Messages orderby m.Id descending select m).Take(count);
            /*
            foreach (var msg in db.Messages.ToList())
            {
                Console.WriteLine("inside get loop");
                Console.WriteLine(msg.Id);
                Console.WriteLine(msg.Content);
                Console.WriteLine(msg.UserId);
            }
            Console.WriteLine("outside get loop");

            //return Ok("Get метод");*/
            return Json(selectedMessages);
        }
        [HttpPost("/writeMessage")]
        public IActionResult Write(string username, string message)
        {
            using (Models.ApplicationContext db = new Models.ApplicationContext())
            {
                var userId = from u in db.Users where u.Login == username select u.Id;
                Models.Message newMessage = new Models.Message { Content = message, UserId = userId.FirstOrDefault() };
                db.Messages.Add(newMessage);
                db.SaveChanges();
                /*
                foreach (var msg in db.Messages.ToList())
                {
                    Console.WriteLine(msg.Id);
                    Console.WriteLine(msg.Content);
                    Console.WriteLine(msg.UserId);
                }*/
            }
            Console.WriteLine("write here");

            return Ok("Сообщение записано в БД.");
        }
    }
}
