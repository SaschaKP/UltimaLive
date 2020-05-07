UltimaLive v.0.98

Introduction

The purpose of UltimaLive is to allow a shard to edit its maps while the game is running, stream those changes to its players in realtime, and use more than six map files.

Map Editing and Streaming

UltimaLive provides shard developers the ability to alter maps without the hassle of redistributing map files to their entire player base. This gives shard developers the possibility of scripting events that make changes to the game maps while the shard is running. e.g. If a shard wanted to simulate having a meteor crash into the terrain for example, they could script in the effects and actually make a crater in the map.

UltimaLive allows shard administrators the ability to edit the terrain and statics in realtime and see those changes immediately. Items can be frozen into the map while the game is running and the maps stored on the players computer are automatically updated when they get near the changed part of the map.

Players are not required to download shard maps at all if they would like to stream all the maps. On the other hand, shards can still distribute maps to their players and in that case players would only be sent parts of the map that have changed since the maps were released. They would only be streamed those changes when they came into contact with the part of the map that had changed.

Additional Maps

The regular Ultima Online client supports six map files. While shards can currently use those six map files to make as many logical maps (copies of a map file) as they want, all the logical maps will look the same.

UltimaLive allows shards to use more than the regular six map files. They can create upwards of 200 additional unique maps, which means the possibility for expansion has been greatly enhanced.

If shards only want the ability to use additional maps and not stream changes, the streaming can be disabled. Additional Maps have the limitation of being the same size (same number of blocks) or less than the post-ML Fel and Tram maps.

Limitations
The following are limitations of this system:

UltimaLive only works with the classic client at this time.
UOP Maps must have the same number or fewer blocks than the regular Felucca map.
Map diff files are not used by this system
There is no provision for wrapping in the boat system. Please explore this and post results.

System Requirements

Tested with various clients upto and including 7.0.30.3
Supports Windows XP**, Vista*, Windows 7, and Windows 8*, Windows 10

*Testing has not been done on Vista and 8, but there is no reason to believe it won't work.

** Microsoft is no longer supporting Windows XP. UltimaLive has been tested on XP, but development going forward will not be tested on XP.


Client Side Installation Instructions

Rename Igrping.dll in the client folder to Igrping_.dll.
Copy Igrping_0_98.dll from the UltimaLive release into the client folder.
Rename Igrping_0_98.dll to Igrping.dll

The client will load the UltimaLive version of the Igrping.dll file, which in turn calls the Igrping_.dll when needed.
You may need to run Ultima Live with administrative privileges when it creates blank maps in the app folder. The client may seem to hang for a few seconds while it creates its blank maps in the app folder. (On Windows 7 this is typically located at c:\ProgramData\UltimaLive\ShardIdentifer)
Maps are stored in ProgramData\UltimaLive\shardidentifer for windows 7. (Windows XP uses a different location for its app data)

Server Side Installation Instructions - Basic
Drop the included server files into your customs folder.
Each shard should have a unique identifier. Alter UltimaLiveSettings.cs and change the unique shard identifier.
Make the following modifications to PlayerMobile.cs
find the class declaration of PlayerMobile:
Code:
  public class PlayerMobile : Mobile, IHonorTarget
Add the keyword partial before the keyword class:

Code:
  public partial class PlayerMobile : Mobile, IHonorTarget
Look in playermobile and find the SetLocation method and the OnMapChange method.
Code:
  public override void SetLocation( Point3D loc, bool isTeleport )
Code:
  protected override void OnMapChange( Map oldMap )
Add this to the bottom of the SetLocation method and to the top of the OnMapChange method

Code:
  /* Begin UltimaLive Mod */
  if (BlockQuery != null)
  {
  m_PreviousMapBlock = BlockQuery.QueryMobile(this, m_PreviousMapBlock);
  }
  /* End UltimaLive Mod */
You will need to go into the MapRegistry.cs and define the additional physical maps you want to use. You can see existing examples in the file.
You will also need to define maps in your regular server/misc/mapdefinitions.cs, for example

Code:
  AddMapDefinition(33, 0, new Point2D(181, 181), new Point2D(181, 181));
Here is a screenshot of what the streaming looks like:

And another:

You can see the green portion on the minimap has not been explored yet. The portions that are visible have been streamed and stored in the app data folder.
Here is a youtube video showing the streaming portion of UltimaLive:


Here is a youtube video showing some of the early development of the system. It does show in game map editing (a small amount).

Server Side Installation Instructions - Core Mod for hued statics support
In TileList.cs around line 59, there is a method called Add
e.g.
Code:
  public void Add( ushort id, sbyte z )
Right below that method add the following method which takes a hue as an argument.
Code:
  /* Begin UltimaLive Mod */
  public void Add(ushort id, sbyte z, short hue)
  {
  if ((m_Count + 1) > m_Tiles.Length)
  {
  StaticTile[] old = m_Tiles;
  m_Tiles = new StaticTile[old.Length * 2];
  for (int i = 0; i < old.Length; ++i)
  m_Tiles[i] = old[i];
  }
  m_Tiles[m_Count].m_ID = id;
  m_Tiles[m_Count].m_Z = z;
  m_Tiles[m_Count].m_Hue = hue;
  ++m_Count;
  }
  /* End UltimaLive Mod*/
In TileMatrix.cs around line 426 in the ReadStaticBlock method
e.g.
Code:
  private unsafe StaticTile[][][] ReadStaticBlock( int x, int y )
method, you should see a line:

Code:
  lists[pCur->m_X & 0x7][pCur->m_Y & 0x7].Add( pCur->m_ID, pCur->m_Z );
replace that line with

Code:
  lists[pCur->m_X & 0x7][pCur->m_Y & 0x7].Add( pCur->m_ID, pCur->m_Z, pCur->m_Hue );
Recompile your server core.

Commands
There are several commands that can be used to alter the map while the game is running. They should be used as a basis to create better, more user friendly methods for editing the map.
[IncStaticAlt relative height
[SetStaticAlt absolute height
[SetStaticID graphic number
[DelStatic
[AddStatic
[MoveStatic
[IncLandAlt relativeheight
[SetLandAlt absolute height
[SetLandID graphic number
[CircularIndent relative height
[LiveFreeze
[incsy Increase the y coord of a static
[incsx Increase the x coord of a static
[setstatichue set the hue value of a static
These are debug commands:
[GetBlockNumber
[QueryClientHash
[UpdateBlock

Credits
Code that I have used in this project:
Xenoglyph for parts of MasterControl
Márton Anka for mhook
The folks at RunUO.com that have helped me on this project.
fwiffo for patches, testing and feedback
Eric T. Benoit for additional commands, testing and feedback
GoldDraco13 for his gump project that supports UltimaLive
-hash- for undocumented client features and packets
Zippy for writing Razor that allowed me to write the first versions of UltimaLive
Warstone for many of his ideas
The folks at UODEV.DE for UOP information
Everyone who has tested and provided feedback about this system.
If you want special recognition for your help on this project, just send me a PM to remind me and I will gladly add you to the credits.

Change Log
v0.98
Released under the MIT License
Lots of ClientSide cleanup
Fixed a number of warnings throughout the codebase
Improved encapsulation of Client class
Client internal structure searching done all at once up front, abort UltimaLive upon failure to find any internal client structures
Removed many unnecesary casts and changed memory searching functions to return void*
Added internal client structure definitions and made searching and calling functions have stronger type enforcement
Added self registering class mechanism to provide for adding classes without modifying existing code
Moved memory allocation from FileManager child classes into the base class
Eliminated most warnings

v.0.97
Fixed a bug that caused some file handles to be closed twice
Fixed a bug that was causing map and graphic glitches on maps that were smaller than Felucca
Moved Client files save path into the UltimaLiveSettings.cs file to centralize it
Added path for lumber harvest system to the UltimaLiveSettings.cs file. If you are not using the system, you can remove these lines of code or leave them.
Cleaned up various warnings
Fixed a bug that was causing map operations to conflict
Moved ServerSide files into a subfolder called Core in preparation for systems that will leverage UltimaLive
Added Eric T. Benoit's modification to AddStaticCommand
Moved some of the PlayerMobile edits to PlayerMobilePartial.cs. This will require the player to mark the PlayerMobile class as partial, but will require less overall edits.
Added Eric T. Benoit's Set Hue Command
Added Fwiffo's fix for Tile sorting
Added Fwiffo's Fix for Client file export
Added Fwiffo's Fix for LiveFreeze command
Added Eric T. Benoit's incsx and incsy commands for moving statics
