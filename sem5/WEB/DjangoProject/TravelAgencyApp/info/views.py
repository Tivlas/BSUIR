from django.shortcuts import render
from .models import News, Faq

# Create your views here.


def main_view(request):
    latest_news = News.objects.latest('id')
    return render(request, "main.html", {'latest_news': latest_news})


def about_view(request):
    return render(request, "about.html")


def news_view(request):
    news = News.objects.all()
    return render(request, "news.html", {'news': news})


def faq_view(request):
    faq = Faq.objects.all()
    return render(request, "faq.html", {'faq': faq})
