package com.example.distributedsystem;

import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.security.oauth2.core.user.OAuth2User;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ModelAttribute;

import java.util.ArrayList;
import java.util.List;

@ControllerAdvice
public class GlobalController {

    // Get the email of the logged in user
    @ModelAttribute
    public void getUserEmail(@AuthenticationPrincipal OAuth2User principal, Model model){

        List<String> facultyEmailAddress = new ArrayList<>();
        facultyEmailAddress.add("maurieslopez1800@gmail.com");

        if (principal != null) {
            if ( !facultyEmailAddress.contains(principal.getAttribute("email"))){
                String email = principal.getAttribute("email");
                model.addAttribute("userEmail", email);
            } else {
                String email = principal.getAttribute("email");
                model.addAttribute("facultyEmail", email);
            }
        }
    }
}
