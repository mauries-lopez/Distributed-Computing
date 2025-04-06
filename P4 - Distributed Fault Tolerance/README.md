# **Online Enrollment System**
This system is implemented using different [microservices](https://www.geeksforgeeks.org/are-microservices-distributed-systems/) distributed across multiple nodes (virtual machines) that are connected within the same network. It has fault tolerance, meaning that if one feature goes down, the rest of the features remain fully functional.

> A demonstration of the system is shown in `Demonstration.mp4`

## **System Overview**
### Architectural Pattern
The system uses a [Microservices](https://www.geeksforgeeks.org/are-microservices-distributed-systems/) architectural pattern where the services or nodes communicates through the use of REST APIs.
### Communication Protocol
The system uses HTTP as a way of web communication.
### Data Management
The system uses [MongoDB]((https://www.mongodb.com/try/download/community)) as its data storage.
### Security Considerations
The system uses [OAuth](https://developers.google.com/identity/protocols/oauth2/web-server) with CSRF Protection (utilizes XSRF-TOKEN/CRF TOKEN).
### Scalability and Performance Optimization
The system utilized [Horizontal Scalling](https://www.geeksforgeeks.org/system-design-horizontal-and-vertical-scaling/) as it employs multiple virtual machines, allowing them to act as features of the web application.
### Fault Tolerance
The system utilizes Horizontal Scaling. Hence, regardless of whether one virtual machine (node) is offline, other features hosted on different virtual machines will remain online, and the web application will still be usable as is.
### Session Tracking
The system stores the `JSESSIONID` of the user in cookies upon successful login through OAuth.

## **Features**
1. **Login/Logout**
2. **View Available Courses**
3. **Students can:**
   1. **enroll to open courses**
   2. **view their previous grades**
4. **Faculties can:**
   1. **upload a grade for a specific student**
   2. **deploy a new course** *(Addtional)*

> A total of **6 features** are implemented, with 5 being the bare-minimum features and 1 additional feature.

## **Prerequisites**
- [Java SE Development Kit 22](https://www.oracle.com/java/technologies/javase/jdk22-archive-downloads.html) or later versions
- [Gradle](https://gradle.org/next-steps/?version=8.13&format=all)
- [MongoDB Community Server](https://www.mongodb.com/try/download/community)
- [VirtualBox](https://www.virtualbox.org/)
- [IntelliJ IDEA](https://www.jetbrains.com/idea/)

## **Disclaimer**
> This application is **locally interconnected** through a network, meaning the machines communicate using <u>**hostnames and IPv4 addresses**</u>. As a result, the redirect URLs are hardcoded based on the specific machine used during testing. Therefore, <u style="color: lightgreen;">**you may need to update these redirect URLs according to your own virtual machine setup on the code-based.**</u>

> The microservices are exported as **.jar** files, which are then transferred across different virtual machines (nodes) to act as services for the whole web application. These are already provided in a single folder called `JAR files` within the directory to be [drag and drop](https://www.thewindowsclub.com/how-to-enable-drag-and-drop-support-in-virtualbox) to the virtual machines.

##
> Make sure you have prepared <u>**atleast 5 virtual machines**</u> before proceeding.
##

## **(1) Preparation**
> The system's features are distributed to different nodes. Thus, nodes should be consistent and standardized.
1. All of the virtual machines have [Java SE Development Kit 22](https://www.oracle.com/java/technologies/javase/jdk22-archive-downloads.html) or later versions installed.
2. All of the virtual machines are using [Bridged Network](https://serverfault.com/questions/225155/virtualbox-how-to-set-up-networking-so-both-host-and-guest-can-access-internet). This can configured in the virtual machine's settings.
3. <u>One</u> of the virtual machines have [MongoDB Community Server](https://www.mongodb.com/try/download/community) installed. This node will be treated as the <u>database</u>.


## **(2) Updating the Redirect URLs**
> This is necessary because the redirect URLs are hardcoded based on the specific machine used during testing.
1. Open `distributedsystem` folder in [IntelliJ IDEA](https://www.jetbrains.com/idea/)
2. Go and open `distributedsystem > src > main > java > com.example.distributedsystem > APIController.java`
3. Change the `hostname` initialization value to the [`hostname`](https://www.configserverfirewall.com/windows-10/find-hostname-in-windows/) of the virtual machine you want to use. For each feature, make sure to update the hostname variable in all the functions it uses.
   - Available Course Feature
     - *reqAvailableCourses()*
   - Enroll Course Feature
     - *reqEnrollCourse()*
     - *submitEnrollACourse()*
   - View Previous Grades Feature
     - *reqViewPrevGrades()*
   - Upload Grades Feature
     - *reqUploadGrades()*
     - *submitUploadGrade()*
   - Deploy Course Feature
     - *reqDeployNewCourse()*
     - *submitNewCourse()*
4. Once done, proceed to change the redirect URL of each feature to the database by opening each folder in IntelliJ IDEA, except for `distributedsystem` and `database` folders.
5. In each feature's controller, change the `hostname` initialization value to the [`IPv4 Address`](https://www.howtogeek.com/858334/how-to-find-your-ip-address-from-cmd-command-prompt/) of the virtual machine you want to use as the database.

> Once all redirect URLs are updated in the **APIController.java** and the **feature controllers**, proceed on exporting them as a `.jar` file. You can use the following command in each of their own terminal `./gradlew clean bootJar` to export them as jar files. The .jar file is saved in this directory `<"feature">/build/libs/`.

## **JAR Files**
- General Feature
  - `distributedsystem-webapp.jar` - User Interface with OAuth 2.0
- Student Features
  - `availablecourse-feature.jar` - Available Course Feature
  - `enrollcourse-feature.jar` - Enroll Course Feature
  - `viewprevgrades-feature.jar` - View Previous Grades Feature
- Faculty Features
  - `deploycourse-feature.jar` - Deploy Course Feature *(faculty)*
  - `uploadgrades-feature.jar`- Upload Grades Feature *(faculty)*
- System
  - `database-host.jar` - Database using MongoDB

## **(3) Put a JAR file in a Virtual Machine**
> This step ensures that each node is responsible for acting as a feature of the web application. This is also where fault tolerance comes into play.
1. Put any `.jar` file you want into the virtual machine through [drag and drop](https://www.thewindowsclub.com/how-to-enable-drag-and-drop-support-in-virtualbox). Except `distributedsystem-webapp.jar`, as I recommend it to be placed on your host/own machine instead of a virtual machine.
2. After all machines contains a `.jar` file, run it through the cmd in its directory using the command `java -jar <'filename'>.jar`. If successful, the cmd will show a ASCII logo of SpringBoot (framework used).
3. Then, you can now visit `localhost:8080` on any browser of your liking.

> *Note: you can turn off a feature by spamming `Ctrl+C` in its cmd.*

## **(4) Logging in**
> The system uses [Spring Boot and OAuth2](https://spring.io/guides/tutorials/spring-boot-oauth2) which allows it to have an authorized login system.
- You can use any email account to login. However, this will be treated automatically as a **Student** account, so only features for students will be visible."
- To have a faculty account, you may add your personal email address in the code by hard coding it in `distributedsystem\src\main\java\com\example\distributedsystem\GlobalController.java`.
  - *Note: You may need to re-export the `distributedsystem-webapp.jar` again for the new changes to apply and re-run the .jar file in the command prompt.*

## Overall Major Dependencies
- Spring Web
- Thymeleaf
- Spring Data MongoDB
- Lombok
- Jackson
- OAuth
- REST API