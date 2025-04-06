package com.example.deploycourse;

import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.client.RestTemplate;

import java.util.HashMap;
import java.util.Map;

@RestController
public class DeploycourseController {

    private final RestTemplate restTemplate = new RestTemplate();

    @PostMapping("/reqDeployNewCourse")
    public String reqDeployNewCourse(){
        return "deployCourse";
    }

    @PostMapping("/submitNewCourse")
    public String submitNewCourse(@RequestBody Map<String, String> newCourseData){
        // Hostname of Available Courses Node
        String hostname = "192.168.55.144"; // <--------------------------------- CHANGE HOSTNAME to VM's HOSTNAME (run "hostname" on cmd at VM)

        // Send to Available Courses Node
        String tempUrl = "http://" + hostname + ":8080/submitToDBNewCourse";

        // Create Object using Map
        Map<String, String> newCourseDataToDB = new HashMap<>();
        newCourseDataToDB.put("courseID", newCourseData.get("courseID"));
        newCourseDataToDB.put("courseName", newCourseData.get("courseName"));
        newCourseDataToDB.put("numTotalSlots", newCourseData.get("numTotalSlots"));

        String response = restTemplate.postForObject(tempUrl, newCourseData, String.class);
        return response;
    }
}
