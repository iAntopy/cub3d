 CUB3d
-   Reproduce a mimic of a Icon Game : WOFLSTEIN 3D

## Mandatory Part

-    __Made Using Codam MLX 42__

- - - - - 
### Keybinding

        * W,A,S,D : move
        * Left & Right Arrow  : Directional keys to rotate camera
        * Up & Down Arrow :  Zoom in & out   
        * ESC to quit 

### Textures

        * Textures & colors differents for each sides 
            * North: N, South: S, West: W, East: E
        * Ceilling: C, Floor: F
            * Color (R, G, B) set as [0,255]
                * Ex "225, 110, 10"

### Map request

        * Containing only selected Char "01WNES"
            * Surround by Wall
                * '0' Spaces between '1' Walls
            * Position Player Letter
                * 'W,N,E,S' Direction to 'Look_ At'

#### Any Misconfiguration 

-           should lead to an descriptive "Error\n" Message

  
- - - - 
## IMG
<p align="center">
<img width="256" alt="Screenshot 2023-05-13 192735]" src="https://github.com/iAntopy/cub3d/assets/88173405/843a9588-8300-4038-adf8-771e97aee45d">
<img  width="256" alt="Screen Shot 2023-06-06 at 1 31 22 PM" src="https://github.com/iAntopy/cub3d/assets/88173405/43f10513-4c53-4064-bc8a-fbe8e9ad8678">
<img width="256" alt="Another World" src="https://github.com/iAntopy/cub3d/assets/88173405/4b700c21-6b18-4bd3-9914-0ca2bee781a2">

</p>

<p align="center">
<img  width="256" alt="Screen Shot 2023-06-08 at 7 54 33 PM" src="https://github.com/iAntopy/cub3d/assets/88173405/b2287f1a-f1db-4dc0-b24a-0aa66bca4222">
<img  width="256" alt="Mars Screen Shot" src="https://github.com/iAntopy/cub3d/assets/88173405/229b4b88-532a-4115-8e12-f50161422f98">
<img  width="256" alt="Field by night" src="https://github.com/iAntopy/cub3d/assets/88173405/3970b4db-5f0d-4cba-8685-55edaa9ba406">
</p>

- - - - 
#### Making of

<img align="left" width="256" alt="Screenshot from 2023-03-09 15-57-44]" src="https://github.com/iAntopy/cub3d/assets/88173405/bd7ab69e-d638-4190-93fd-9268f2515025">
<img align="center" width="210" alt="Screen Shot 2023-05-15 at 3 22 16 PM" src="https://github.com/iAntopy/cub3d/assets/88173405/f995be1d-3547-45ce-9b75-fa4efd4868ff">
<img align="center" width="220" alt="Screen Shot 2023-06-06 at 1 14 12 AM" src="https://github.com/iAntopy/cub3d/assets/88173405/c0c61912-bcba-4e22-a191-dddbb95d401c">
<img align="center" width="180" alt="last of them" src="https://github.com/iAntopy/cub3d/assets/88173405/85a10ac3-68fd-4f74-ad39-b20a76f9355b">

- - - - - -

    ## Bonus Parsing
>>
>>    **FILE HEADERS**
>> - - - - 
>>    ### Object
>>      Meta-Char-Part   
>>          Object represent into Map
>>>                  EX:__ ! 1/< __
>>              
>>-     First 'char[0]' 
>>         Ref. Char.  followed by space.
>>-      Second 'char[2]' 
>>         Alleg. Num. followed by slash.
>>-     Third 'char[4]'
>>         Relativ. Char. ... or *directional.
>>
>> - - - - 
>>    ### Legend
>>        Legend-Part--Lowercase   
>>            Unique texture to be set further
>>>                  EX:__ a tex/ext/wood_plank.png __
>>-     First 'char[0]' : Name
>>         Textr. Char.  followed by space.
>>-      Second Char*   : Path 
>>         Textr. char* fullpath ended by .png
>>
>> - - - - 
>>    ### Horizontal
>>        Dual-Part--Numeric
>>          Paired Texture floor and ceiling 
>>>              EX:__  0 tex/ext/floortile.png tex/ext/tile_ceiling_.png __
>>-     First 'char[0]' : Numeral Name view into map
>>         Textr. Num.  followed by space.
>>-      Second Char*   : Path Floor 
>>         Textr. char* fullpath ended by .png
>>                      followed by space.
>>-      Third  Char*   : Ceilin Floor 
>>         Textr. char* fullpath ended by .png
>>    
>> - - - - 
>>    ### Preset
>>        Pset-Part--Uppercase
>>          A Set of Wall Texture 
>>>              EX:__  A abcd __
>>-     First 'char[0]' : Char name view into map
>>         Textr. Char.  followed by space.
>>-      Second Char[2]   : West side Lowercase ref. Txtr
>>-      Third  Char[3]   : North side Lowercase ref. Txtr
>>-      Forth  Char[3]   : East side Lowercase ref. Txtr
>>-      Fifth  Char[4]   : South side Lowercase ref. Txtr
>> - - - - 
>>  ## Map
>>
![header](https://github.com/iAntopy/cub3d/assets/88173405/ba4d94a5-b775-46f7-b957-b2758ebf1208)
![map1](https://github.com/iAntopy/cub3d/assets/88173405/23581384-2504-4ef1-9847-ab4721d8191f)
>> - - - -
>>>
>> - - - - 
>>  ### Lexical
>>      Meta-char family [ MCHR ]
>>          Lever   : (+,!,)
>>          Portal  : (<,>,_,^)
>>          Firpit  : (=,;,:)
>>          Fireball: (*,#,%)
>>          Player  : (@,$, &)
>>          Spec    : (?)

