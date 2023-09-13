from django.shortcuts import render, redirect
from django.views.generic.edit import FormView
from .forms import MyUserCreationForm
from django.contrib.auth import logout, login
from django.contrib.auth.forms import AuthenticationForm


class SignUpFormView(FormView):
    form_class = MyUserCreationForm
    success_url = '/login/signin/'
    template_name = 'signup.html'

    def form_valid(self, form):
        form.save(True)
        return super(SignUpFormView, self).form_valid(form)

    def form_invalid(self, form):
        return super(SignUpFormView, self).form_invalid(form)


class LogoutFormView(FormView):
    def get(self, request):
        logout(request)

        return redirect('/')


class SignInFormView(FormView):
    form_class = AuthenticationForm
    success_url = '/'
    template_name = 'signin.html'

    def get(self, request):
        return render(request, 'signin.html', {'form': self.form_class})

    def form_valid(self, form):
        self.user = form.get_user()
        login(self.request, self.user)
        return super(SignInFormView, self).form_valid(form)
