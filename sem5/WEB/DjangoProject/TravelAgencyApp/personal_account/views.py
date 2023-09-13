from django.shortcuts import render
from django.contrib.auth.decorators import login_required

@login_required(login_url='/login/signin/')
def personal_account(request):
    user = request.user
    orders = user.order_set.all()
    return render(request, 'personal_account.html', {'user': user, 'orders': orders})
