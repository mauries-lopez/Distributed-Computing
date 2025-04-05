package com.example.uploadgrades;

import org.springframework.core.ParameterizedTypeReference;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.client.RestTemplate;

import java.util.HashMap;
import java.util.Map;

@RestController
public class UploadgradesController {

    private final RestTemplate restTemplate = new RestTemplate();

    @PostMapping("/reqUploadgrades")
    public String uploadGrades(){
        return "uploadGrades";
    }

    @PostMapping("/submitUploadGrade")
    public String submitUploadGrade(@RequestBody Map<String, String> gradeData){
        // Hostname of Available Courses Node
        String hostname = "192.168.55.144"; // <--------------------------------- CHANGE HOSTNAME to VM's IPv4 ADDRESS hosting the DATABASE

        // Send to Available Courses Node
        String tempUrl = "http://" + hostname + ":8080/submitToDBUploadGrade";

        // Create Object using Map
        Map<String, String> gradeDataToDB = new HashMap<>();
        gradeDataToDB.put("email", gradeData.get("email"));
        gradeDataToDB.put("courseID", gradeData.get("courseID"));
        gradeDataToDB.put("courseGrade", gradeData.get("courseGrade"));

        // Send request and receive response
        String response = restTemplate.postForObject(tempUrl, gradeDataToDB, String.class);

        return response;
    }
}