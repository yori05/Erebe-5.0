# Erebe-5.0 (last update on 06/01/23)
Project initially Unreal 4.26 migrate to 5.0 using blueprint and c++ when needed.
Currently in Unrealv5.1

All the class of this project (or the majority of these class) are made in c++, but can be (and sometimes should be) override in blueprint.
They pocess callback, when necessary blueprint event function, and if need should be used.
This project is at the moment my work (Florian Hulin) only (if we don't count the fact I use and over use unreal engine and the work of the epic game team on it, and visual studio) 
You should use part of this project without my autorisation, but you can look all you want. 
I ask for you forgiveness for my typo, english is'nt my native language, I think some will obviously find their way in my text, I will be more than happy to fix them if pointed.

For the unreal part, I currently exploit the world partition system on one map : Content/Maps/TimeManipulation/[TM_Map_WP]
I use self made and free asset or at last one provide by unreal starter pack. In case you find asset owning by you, you are free to say it to me and I will add your name on it or remove it if you want.
For now this project is far from being something else then a personnal project.
If you find part of my code needing more comment feel free to ask, I will add them and explain the action take in the code.

I will try to keep this readme up to date with every commit take.

This project contain :
-> Dialogue system (base) :                 [UDialogueComponent_Basic]
    Use a structure to save dialogue information in a data table, and load data on a Dialogue component at the begin play
    Let display line for dialogue, the data can be export from a excel file
    Can have multiple coversation, and line for each conversation, use a Speaker ID to load every line on a data table with a dialogue component to let
      actor using this component exploit these line
    Can use Speaker name with each dialogue if need
    Use event in case dialogue line have a SpecialEvent set as true 
    For now don't use the localization system for unreal
    
-> Signpost system (base) :                                         [UIC_Signpost_Basic]
    Use a structure to save signpost information in a data table, and load data on a signpost component at the begin play
    Can display the saved line, the data can be export from a excel file
    For now don't use the localization system for unreal
    For now this component is part of the Interaction system, need to split it to have a component to just do the signpost job.
    
-> Door system (base) :                                             [UDoorComponent_Basic]
    Use a FloatCurve to manage a door component using a RatioValue (as a float) to know if the door in open (1.f), close (0.f), openning, closing.
    Can block the door, blocking it from being open or close. Or stuck in a opening / closing state.
    For now door are set as close.
    Can use a reverse movement to open, saving the door as being open in both direction. 
    
-> Time manipulation system (intermediate) :
    --> Time manipulator component manager :                        [UTimeManipulatorComponent_Manager]
        Use a on a game mode actor, to manage action for every actor using this system.
        Save all Time Manipulator Component to call action to stop, record and replay action on actor using these component
        Link with these component adding theme to an array and bind to callback to switch mode.
    --> Time manipulator component :                                [UTimeManipulatorComponent_Basic]
        Use Time manipulator component on actor using this system
        The base for these component just follow order from the manager, to record, replay, hold time for every component using this system
        Keep array of curve for every kind of information manage by this component.
        Use the curves to record and replay action.
        ->  The PhysicBasic :                                       [UTMComponent_PhysicBasic]
            Child of the base component and will record / replay movement (rotation / translation / and scale) for actor using this component 
            Create curve for each, can unhandle info (ex : in case we don't want to record scale, won't create a curve for it, will not record it, nor replay it, set as false for the scale)
            For now velocity is'nt take in charge by it, so in case you use it and stop the replay in half of a movement this movement will be hold.

-> Interaction system (intermediate) :
    Base for using it in lot of case, 
    --> InteractiveComponent Basic :                                [UInteractiveComponent_Basic]
        Should be used on an actor to implement interaction action with an other component or this actor 
        Can use a requestor when trying to interact with it, use a enum to save the interaction type of the component
        Can implement action in c++ or blueprint 
        Should be use as Sub class to implement linked action (ex : [UIC_DoorOpenner_Basic], [IC_Dialogue_Basic], ...)
    --> InteractiveComponent Manager :                              [UInteractiveComponent_Manager] 
        Used to manage single or multiple interactive component on an actor and if one is set the interaction area
        Keep interactive component add on the actor, hold a main interaction (in case on is set as) 
        Can search on the actor a PrimitiveComponent used as the interaction area and use it for :
        -> Launch main interaction when an actor or the player begin overlap with it
        -> Stop main interaction when an actor or the player leave overlap with it
        Save one interactive component for each interaction type (can use custom interaction type in case we don't add an element in the enum)
        
->  Custom Base Character (base) :                                  [ACharacter_Basic]
    Base Character for player or ai, contain movement in every direction and jump.
    Pocess two skeletal mesh for third person and first person utilisation, use viewpoint and move a camera to follow these viewpoint while switching view mode
    As a base can interact with the interaction system, for now using simple order.
    Can use action while called from a controller using the c++ and blueprint action (for now you can choice to use only one both are called)
    Should be use as a Sub class to implement more specific action for each kind of gameplay
    Will implement character state like in [AErebeCharacter] on a child class later
    
->  Custom Base Player Controller (base) :                          [APlayerController_Basic]
    Interact easly with a character basic,
    Will catch input and call action on the Character pawn, or other actor in case needed
    Ex : [APlayerController_TimeManipulator] will call action on the TimeManipulator on the manager to launch the hold, replay, record time for the whole level.
    
->  Weapon system (base) [old] :                                    [AWeapon_Base]
    Use a base system to implement gameplay for weapon whome will follow the actor using them
    Pocess two mesh in case we want to use it on a actor with a third and first person mesh.
    Will be link to actor as a owner, can use owner viewpoint and impact point as orientation and location for the weapon effect
    --> Weapon shooting :                                           [AWeapon_Shooting_Base]
        Pocess Ammo and Fire mode,
        Use a pool of Ammo for every weapon, will spawn at the same time as the weapon the pool and will use it to use the weapon
        Will save the ammo in two array to keep tract of projectil avaible, and projectil spwanned
        Can spawn projectil on a specific socket if name is specified, or at the impact point specified.
     Even if both class are set as old they are the most recent one before the change of working management 
     Both class are 'old' they were made before the Custom Character Base, should be edite later to be easly used and link with the new character system
     Should still be used easly with every kind of actor, but a child of these class could be mate, or a component to link easy the weapon action with specific  kind of actor

->  Ammo system (base) [old] :                                      [AAmmo_Base]
    Base for ammo compatible with weapon shooting base
    They pocess a projectil movement component, static mesh and sphere collider.
    They can set as inactive when hit something, and if set as true stick to the actor hit.
    They can use a custom physic to move.
    
->  Widget (User Interface) : 
    For now two widget are avaible :
    --> Dialogue :                                                  [UUW_Dialogue]
        Will search for all Dialogue components on the scene on begin play and link to them
        When link to component the widget will add all of them in an array and bind display function to their callback (when the line change, begin display, end display)
        In case we need it Dialogue component can search the dialogue widget and request to link to it.
        We can block the widget from searching for the components on the begin play if we need.
        This widget is design with meaning of having one widget for all dialogue compoennt, in case we want to have a widget for each, or unique dialogue component we need to send the compoennt manualy.
        Note that a Dialogue widget can be link to multiple Dialogue component and a dialogue compoennt can be link to multiple widget.
        Will break link one of the two are destroy or remove from the game. 
        Maybe will add a static reference for the widget if we want to use a unique one and easier the access when component try to access it.
    --> Signpost :                                                  [UUW_Signpost]
        Will do the same job than with de dialogue components except he do it with a signpost component.
        Will search for all Signpost components on the scene on begin play and link to them
        When link to component the widget will add all of them in an array and bind display function to their callback (begin display, end display)
        In case we need it Signpost component can search the signpost widget and request to link to it.
        We can block the widget from searching for the components on the begin play if we need.
        This widget is design with meaning of having one widget for all signpost compoennt, in case we want to have a widget for each, or unique signpost component we need to send the compoennt manualy.
        Note that a Signpost widget can be link to multiple Signpost component and a Signpost compoennt can be link to multiple widget.
        Will break link one of the two are destroy or remove from the game. 
        Maybe will add a static reference for the widget if we want to use a unique one and easier the access when component try to access it.
    --> GameMode Widget component :                                 [UWidgetComponent_Manager]
        Should be add on the used game mode, will spawn Signpost and Dialogue widget on it.
        When component try to access widget they will search for widgets on this component. 
        He will try to spawn [UUW_Dialogue], etc... or child of these class.
        This component can be add on other actor in case we want specific widget on the owner actor.
        Will add a handle to allow spawn for specified widget.
        Will add a map to save widget with a specified key and access it easly
        



