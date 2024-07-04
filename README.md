# AntiFireSystem

# project description : 
in the industry there's a big risk of fire that damage the resources and the business. For that we initialized this project to Build a mechatronical system that includes AI (artificial intelligence) and IoT (internet of things) to handle this risk.

# IoT PART : 
  
1- Gloabal IoT Diagram :

![345020719-87046f49-a8b1-48dc-b498-d0ebad81a3bf (1)](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/1906c803-8594-412b-9a9b-a43e236dea0c)


2- PART I  (automation layer) : 
      in this part we're going to focus our study on the first layer in our IoT system, the perception layer and we're trying to build sort of embedded systems that called automated objects then make them connected objects by adding the next layer which is the network layer and after that smart objects by including the AI technologies on our embedded systems.
let's build our embedded systems one by one :

  Electrogen Group 1 :
        we use electrogen group to generate electricity in the case of STEG electricity cut.we're trying here to build a prototype that can handle the electrogen group.
        as any embedded system we've 2 parts : an embedded software engineering part & an embedded hardware engineering part. we need to do the design and the realization of the 2 parts
        but we need first of all to do a global overview on our electrogen group system.
  Requirement Sepecification :  
  
  
  synoptic shema :
  
  ![synoptic shema](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/3241109d-8ec8-40b6-aaf9-e6373aeeca7f)

  Global StateMachine Diagram :
  
  ![systeme globale](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/a3e5dff3-68db-454f-86e6-d740d0cb344f)
  ![global table](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/22e9f2f3-be67-4d3e-9297-5c0c7cdd96a4)

  Global Class Diagram : 
  
  ![Global class](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/1ffcebb3-576b-48e6-bbf1-b80bfedd363a)

  embedded software engineering part : 
  ![SDLC](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/721f674c-7192-4740-8181-05db8b63a32a)
          
  Reserve Controlling Sub System :                      
  ![reserve](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/3b538ab0-4ebb-4df6-a386-efddc77e864c)
  ![tab reserve](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/f4c9d30c-5304-449c-9e38-5341262926f1)
  ![tab reserve](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/eb3abf96-05cc-45ad-b30a-ec457a880673)
  ![tab reserve actions](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/94c5362c-bbb7-446d-a8f1-4077c0528133)
  ![reserve1](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/fc2c8510-396d-42c4-ad0a-66380f64b202)

  Temperature Controlling Sub System : 
  ![temp1](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/a252c739-86f8-4fe4-b711-219224fb4766)
  ![tab temperature ](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/e93f9551-6df9-4aeb-9ee5-2772bcc48c93)
  ![temp2](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/b817a5da-fb49-44c1-bde0-3b82b0814911)

  Battery Controlling Sub System :
  
  ![bat2](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/54136746-80ee-4c1d-8667-e15eb7aabe40)
  ![bat table](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/4b14c481-0db2-4eca-a576-97d50d1b3ba9)
  ![bat1](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/f3d59104-9c84-44f0-82f0-6fc64fbb69e6)

  Alert Controlling Sub System : 
  
  ![alert2](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/9f4ac238-794b-4adc-873e-9adc8e56f1b6)
  ![alert1](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/10d98eaf-a12c-471f-9248-2b29f4de0060)

test software in simulation mode :
![simulation auto](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/ee0e6527-2b05-4bdf-8251-8d4bd49769a7)
![simulation man](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/6d80b79e-72b4-4896-9846-45b6dad3740a)


embedded hardware engineering part :
Design :
![shema electronique](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/2716a623-25e0-45fb-873a-e197bc52c1e6)


![shema carte 1](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/085967f6-de51-454f-b758-a1e6b3ded99a)
![shema carte 1 1](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/335c14ce-4c03-4064-8a38-0409b2140b13)

![shema carte 2](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/b39b10f5-b6fd-40df-8d83-145f5c5841ac)
![shema carte2 2](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/3865e305-a57c-4caa-8f5b-5b81c557b84c)

test & realization : 
![testr1](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/0354910c-ae32-41c4-b845-cd5ba7240ed8)![real](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/647e5ddf-6e14-479d-aae0-edee19d93005)

![real 2](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/6730a7c0-ecee-4b0e-ad5d-a69d7ff30bcf)
![real1](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/2bf53857-32c0-411b-be9d-86db4fe4c3a2)
![real3](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/af9f5a8b-b67a-47e6-9f6b-98d642b27da5)
![real4](https://github.com/ssemsOfficial/antiFireSystem/assets/84194047/39174fc7-4d0d-49cc-9276-cda2e0e8b29b)

# AI PART :

