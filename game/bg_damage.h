// bg_damage.h
// Contains all of the code pertaining to custom damage types (meansOfDamage.json)
// (c) 2016 Jedi Knight Galaxies

#pragma once

#include "../qcommon/q_shared.h" 
#include <vector>
#include <string>

#define MAX_OBIT_LENGTH			64
#define MAX_MEANSOFDAMAGE_NAME	64

struct meansOfDamage_t {
	char ref[MAX_MEANSOFDAMAGE_NAME];
	char inventoryName[MAX_QPATH];

	qboolean hitmarkerExplosives;
	qboolean disintegrate;

	struct {
		qboolean noDamagePlums;

		qboolean overrideDamagePlum;
		vec4_t overrideDamagePlumColor;

		qboolean overrideShieldDamagePlum;
		vec4_t overrideShieldDamagePlumColor;

		qboolean overrideLowDamagePlum;
		vec4_t overrideLowDamagePlumColor;
	} plums;

	struct {
		qboolean ignoreArmor;
		qboolean ignoreShield;
		float armor;
		float shield;
		float organic;
		float droid;
		// Jkg improve new modifiers 
		float human;  
		float beast; 
		float lightside; 
		float darkside; 
		float hazardtrooper;
		float sandcreature; 
		float assassindroid; 
		// legacy of Kain modifiers. 
		float ghost; // sluagh and ghosts. 
		float archon; // arconti che servon il dio piovra
		float tentacle; 
		float eldergod;  
		float vampireghost; // vampire ghost 
		// mortal creatures
		float druid; 
		float werewolf; 
		float prophet; 
		float sarafan; 
		float wizard; 
		float cenobita; 
		float melchiahim; 
		float summoner; 
		float melkboss; 
		float zephonim; 
		float deceiver; 
		float zephboss; 
		float dumahim; 
		float dumahboss; 
		float rahabim; 
		float rahabboss; 
		float turelim;
		float tyrant; 
		float turelboss; 
		float vampire; 
		float vampire_demon; 
		float vampireboss; // elder vampires 
		float pureghost; // ariel 
		float darkvampire; 
		float maskvampire; 
		float necrovampire; 
		float hellvampire; 
		float airvampire; 
		float bardvampire; 
		float wolfvampire; 
		float elfvampire; 
		float halfvampire; // half vampire  
		float frostvampire; 
		float archervampire; 
		float razielhim; 
		float razielboss; 
		float halfdemon; 
		float halfmelc; 
		float sarvampire; 
		float shockvampire; 
		float spidervampire; 
		float poisonvampire; 
		float wolfform; 
		float lionform; 
		float hellwolf; 
		float demonwolf;
		float batform; 
		float hellbat; 
		float demonbat; 
		float mistform; 
		float fearmist; 
		float darkmist; 
		float vapormist; 
		float bloodmist; 
		float hellmist; 
		float elfmist; 
		float sarmist; 
		float necromist; 
		float reaver;  
		float reaverboss; 
		float necroreaver;  
		float darkreaver; 
		float tankreaver; 
		float lightreaver; 
		float arcanereaver; 
		float waterreaver;
		float icereaver; 
		float spiderreaver;  
		float firereaver; 
		float airreaver; 
		float mentalist; 
		float stonereaver; 
		float venomreaver; 
		float psychic; 
		float planeshifter; 
		float wraitharmor; 
		float naturist; 
		float energist; 
		float timestreamer; 
		float alchemist; 
		float hylden;
		float hyldenlord; 
		float demon; 
		float demonfire; 
		float demonpoison;
		float demonblade;   
		float demonelectro; 
		float thrall; 
		float poltergeist; 
		float shadow; 
		float voidness; 
		float sun; 
		float water;  
		float crystal; 
		float gale; 
		float earth; 
		float necro; 
		float holy; 
		float poison; 
		float ivy; 
		float sonic; 
		float golemdark; 
		float golemlight; 
		float golemgold; 
		float golemfire; 
		float golemwater; 
		float golemfrost; 
		float golemair; 
		float golemmetal; 
		float golemsonic; 
		float golemholy; 
		float golem; 
		float hellgolem; 
		float necrogolem; 
		float golemsmog; 
		float golemvoid; 
		float golempoison; 
		float golemlava; 
		float ethereal; 
		// Now astrofantasy creatures! 
		// FIRST WORLD: VORKAN, VULCANIC WORLD! 
		float vorkan; 
		float blob; 
		float blobster; 
		float vidra; 
		float carminia; 
		float volvox; 
		float pirosca; 
		float ameboid; 
		float centix; 
		float pkela; 
		float konja; 
		float rak; 
		float pawk; 
		float limulus; 
		float kerlak; 
		float scorpion; 
		float hypopod; 
		float hypomech; 
		float snag; 
		float pirocefalus; 
		float zmija; 
		float vron; 
		float spongia; 
		float torch; 
		float forger; 
		float branote; 
		float shogoth; 
		float jackal; 
		float myriad; 
		float vyvern; 
		float kikima; 
		float skorpius; 
		float pirkar; 
		float volvoma; 
		float vesu; 
		float anorian; 
		float kos; 
		float zhorian; 
		float kromag; 
		float krodan; 
		float gorkan; 
		float sed; 
		float divinator; 
		float cannork; 
		float zarha; 
		float farha; 
		float konga; 
		float sorha; 
		float trikka; 
		float kavel;  
		float striga; 
		float asher; 
		float smoker; 
		float tarter; 
		float rusal; 
		float friggan;
		float clayer; 
		float obsidier; 
		float mudgolem; 
		float irongolem; 
		float puppet; 
		float miniature; 
		float salamech; 
		float pourer; 
		float tetramech; 
		float spidermech; 
		float crabmech; 
		float skormech; 
		float recon; 
		float scramech; 
		float octomech; 
		float torturator; 
		float yulo; 
		float diver; 
		float ignitek; 
		float gladiator; 
		float yerkken;
		float scavenger; 
		float haunter; 
		float screamer; 
		float burner; 
		float flyer; 
		float farg; 
		float kongar; 
		float kef;  
		float washak; 
		float sorr; 
		float keeper; 
		float ziva; 
		float mosok;
		float jura; 
		float dazbor; 
		float devana; 
		float owena; 
		float ivonna; 
		float khor;
		float zwan; 
		float svarg;
		float vesna; 
		float stribo; 
		float jarion; 
		float wik; 
		float magrok;
		float dusah;
		float arrukk; 
		float likho; 
		float veles;
		float babaya; 
		float chuva; 
		float bogatyr; 
		float bozna; 
		float morena; 
		float fefnor; 
		float pekle; 
		float radigost; 
		float zhymar; 
		float tryglav; 
		float horz; 
		float boruta; 
		float borevit; 
		float cockatrix; 
		float bagiennik; 
		float kardag; 
		float cernobog; 
		// DEMETRIUS RACES 
		float demetrius; 
		float cranion; 
		float cefistan;
		float esper; 
		float yoshir; 
		float salomme; 
		float nezo; 
		float linervus;  
		float omh; 
		float jori;
		float gheor; 
		float evros; 
		float memnas; 
		float intellian;
		float selenita; 
		float robopoc; 
	// Demetrius Animals creatures
		float mushroom; 
		float swimalg; 
		float comitre; // musk
		float sponge; 
		float alcyon; // coral 
		float ceterum; // jelly fish  
		float hydra; 
		float plasmoid; 
		float worm; 
		float ubinoto; // verme 
		float geubo; // lombrico 
		float corda; // anfiosso
		float ostreum; // ostrica
		float snail; // cochlea; lumaca
		float nautila; // nautiloide
		float miriapod; 
		float gnatoke; 
		float opabinia; 
		float hallucigenia; 
		float tribby;
		float anomalocaris; 
		float aster; //starfish 
		float wiwaxia; 
		float fish; 
		float bug; 
		float frog; // bufone; 
		float snake; // anguish;  
		float turtle;  // testudina
		float diploid; // diplocaulo 
		float klamida; // klamidosauso 
		float lizard; 
		float raptor;
		float bird; 
		float dedera; // galline
		float camelus; // struzzi
		float sfenish; // pinguini  
		float cincia; // wood bird  
		float feles;  
		float elefos; // elefante 
		float mouse; // topo mus
		float erikus; // riccio
		float tato; // armadillo
		float ungulash; 
		float horse; // equus; 
		float wolf; // lycam 
		float bear; // BEORO, 
		float seal; // 	SIGLISH, foca
		float vesper; // bat
		float deer; //	PYGARGO, rupicapra
		float luter; // lontra 
		float monkey; 
		float lemur; 
		// Demetrius godness
		float mineria; 
		float miras;
		float astreus; 
		float adosh; 
		float olexa; 
		float farash; 
		float efesia; 
		float elish; 
		float maresh; 
		float crisara; 
		float delphon; 
		float lilium; 
		float megatron; // Cranion supercomputer
	// FOR THE ROBOPOC, SEE ZOLTAN LIST. 
	// Khrolen Animals
	  float nubikus; 
	  float aljar; // anguilloide 
	  float almaan; 
	  float shajara; // albero fenice
	  float rophir; // rana alata 
	  float mawray; // murena (serpente marino) 
	  float yuban; // serpente del deserto 
      float bunazar;  
	  float mosox; //mosoide 
	  float ladaq; // ittioide
	  float cralator; 
	  float daktur; 
	  float dik;  // galloide 
	  float saph; 
	  float phatan; //  
	  float dhibia; // mosquito
	  float formox; 
	  float omaan; // ofiurida. 
	  float rambut; // ragno stalatore pungente
	  float sarib; // scorpione tessitore 
	  float tursuk; // pianta carnivora
	// Khrolen Etnic folks
	  float khrolen; 
	  float agula; 
	  float xetol; 
	  float ilzam; 
	  float ebstraa; 
	  float haldaa; 
	  float orgalian; 
	  float jezwar;
	  float shizarin; 
	  float oghta; 
	  float zolag; 
	  float yangha; // navigatori 
	  float xerazim; // antichi isis del passato. 
	  float chor; 
	// magical creatures 
	  float abqa; // genio 
	  float mummy;// non morto   
	  float mayit; // non morto 
	  float ramalen; // spirito delle sabbie
	  float ifrit; // spirito del fuoco
	  float shabal; // fantasma del deserto 
	  float sham; // spirito del sole
	 float najima; // spirito degli astri 
	 float jabal; // gigante della terra
	 float haya; // spirito della vita 
	 float naxa; // spirito serpente
	// godness
	 float tolk; 
	 float onub;
	 float oris;
	 float krif;
	 float xeth;
	 float bestex;
	 float omor;
	 float cork; 
	 float attoth; 
	 float niumun; 
	 //////// ZOLTAN
	 float zoltan; 
	 //Animals
	float pnosa; 
	float irakta; 
	float kuhu; 
	float sucker; 
	float tarthoid; 
	float tricero; 
	float shocksnake; 
	// Planter
	float neuralg; 
	float zoia;
	float ninphal;
	float ninphtower; 
	float cactopod; 
	float xalma; 
	float wooddryad; 
	// Vegeter
	float vegeter;
	float vekla;
	float veknosa; 
	float veghusk;
	float vegnita; 
	float vegpha; 
	float veghem; 
	float groundiver; 
	float vegmolder; 
	float vegpira;; 
	float vegga;
	float vegsna;
	float vekker; 
	float penthas; 
	float vunkta; 
	float adduma; 
	float vegret;
	// ROBOPOC
	float robochoper; 
	float robotruc; 
	float ofidoc;
	float spideroc;
	float crabboc;
	float sgorboc; 
	float octopoc; 
	float recpoc;
	float scarpoc; 
	float trivelpoc;
	float cleaner; 
	float honpoc;
	float phonopoc; 
	float aeron; 
	float gladioc; 
	float cyaner; 
	// SELVARUS 
	// animals
	float slemer; 
	float floven; 
	float khyanl; 
	float bolina; 
	float gnatonk;
	float gnatek;
	float greenwidower; 
	float moum; 
	float memwbaw;
	float meganeura; 
	float kenda; 
	float trey; 
	float krong; 
	float krapthor; 
	float shishara; 
	float psaalab; 
	float krapen; 
	float baochom; 
	float kokob;
	float quetfloatak; 
	float babeo; 
	float thnika;
	float rodent; 
	float kromoon; 
	float smilodon; 
	float tix; 
	float protomoper; 
	float moper; 
	float vatrongo; 
	float elembam; 
	float rinonte; 
	float growa; 
	// monker 
	float monker;
	float vuuru;
	float ling;
	float ligda;
	float bosen;
	float billig; 
	float aara;
	float lugor; 
	float swart;
	float wimda;
	float geesa;
	float saak;
	float aper; 
	// magical creatures
	float comix; // coniglio sacro
	float benix; // floatebra cieca
	float sobatignela; // scorpione malefico
	float ntinti; // foca
	float koko; // delfino
	float mningi; // mostro
	float cipak; // cipactli. krong gigante. afloatteco 
	float tialte; // mostro marino serpente 
	float civata; // civtateo; spiriti non morti del vodoo; dannati; scheletrici; vampirici; donne
	float floatifloatima; // tfloatitfloatimime; i "mostri del cielo"; erano divinità-stelle o demoni.
	float floataund; // spiriti del suono 
	// godness 
	float phlung; // fuoco
	float thamopo; // energia; \ via
	float ponlu; // luce
	float yb; // notte
	float chenga; // foresta
	float sathanga; // animali 
	float bhoomi; // terra
	float tuka; // marini e fiumio
	float khvalla; // spiriti dell'aria serpenti piumati
	float slasma; // morti 
	float wotta; // spiriti delle cose
	// 4 spiriti supremi monker
	float dhom; // armonia
	float nahual; // spirito del bene
	float tecci; // spirito di ovolt; ispirato a teccifloattecatl
	float djanga; 
	//evil aper godness 
	float xaresh; 
	float albar; 
	float zalu;
	float ishmar; 
	float mictla;// demone re dei morti
	float kali; // dea della morte 
	float xolotl; 
	float centfloaton; // conigli infernali 
	float shiba;// la distruttrice 
	// KIRDUS 
	// animals
	float kirdus; 
	float alurko; 
	float alope; 
	float ekinope;
	float pteraspe; 
	float snamper;
	float spirolaid; 
	float spirochete;
	float spirococcus;
	float geit; // capride  
	float rodicornus; 
	float vleer; // caprottero
	float krifter; // gigantostraco
	float sprinkaan; // antrottero
	float glimmer; // emittero - insetti
	float chamlion; // camaleonte iguana
	float hydropside; // idropside
	float haged; // saura; \ rettili
	float pek; // piccacei; uccelli minori
	float gryphon;
	float roc; 
	// etnic nark groups
	float geliot; 
	float bezegoth; 
	float forlok;
	float cogon;
	float arlaan;
	float kwarg; 
	float liter;
	float sonar;
	float monar; 
	float wark; 
	float gork;
	float okur; 
	float netarian; 
	float arkanian; 
	float orc; 
	float satir;
	float nibelhor; 
	// nark magical creatures
	float fair; 
	float shernak;
	float dragon; 
	float dwarf; 
	float goblin;
	float windigo; 
	float blossom; // fiore mutato assassino
	float grotto; // giganti  
	float ogre; 
	float orthas; 
	float gutuk;
	float skeleton; 
	float ghoul; 
	float revenant; 
	float stezaar;
	float reaper; 
	float necrodragon; 
	float salamander; 
	float mutant;
	float blacksatir; 
	float imp;
	float pyrogoth; 
	float vansteen; // golem 
	float zaarsteen; // golem di sangue
	float stonemaster; 
	float occultist; 
	float lich; 
	float arcilich; 
	// eroi supremi 
	float nemedhes;
	float vainamon; 
	float krimhilda; 
	float xengort; 
	float xarzo;
	// godness
	float crowulf; 
	float nemphis; 
	float igner;
	float eikar;
	float aegir; // signore del mare 
	float zon;
	float maan;
	float nephala;
	float aarde;
	float gerucht;
	float jomur; 
	// evil godness \	demons 
	float versterf;
 	float medelus;
	float stilte; 
	float verstand; 
	float grendel; 
	float lok; 
	// KOROT CREATURES
	float korot; 
	float xila;//xalga
	float bromia;
	float pennatula;
	float lasiavus;
	float kitas;//xpiantaxstrangolantex
	float nemnos;//xanemonexgigante
	float gorgia;//xmedusax
	float skoulikos;//xvermex
	float sfoungara;//xfungoxsottomarino
	float kito;//xmolluscox
	float xituwa;//xlumancax
	float skrilapos;//xpolpox
	float satantopod;//xscolopendra
	//xbruchix
	float kampia;
	float mirikasti;
	float chelato;//xbrucoxcrostaceo
	float epidromeo;//xnecrofagox
	float gigantostraco;
	float skorpiopode;
	float sukoka;//xscorpionexcoccodrillox
	float kavuro;//xgranchioxgigante
	float acklante;//xgranchioxmantidexacklay
	float katfloataro;//xpescexriccio
	float angura;//xcetrioloxdixmarexx
	float krino;//xgiglioxdixmara
	float thalassa;//xsanguisugaxbloodgaufloater
	float ascidia;
	float psar;//xpesce
	float verda;//xstorione
	float celacan;//xcelacanto
	float dinyctus;//xpescexmostro
	float karcharia;//xsqualoxcorafloatato
	float placo;//xostracodermaxpescexcorafloatfloatato
	float estemmo;//xestemmosucox
	float diploderma;
	float krimaka;//xserpexgigantex
	float savra;//xlucertolaxaxseixfloatampex
	float vrissak;//xvaranoxaxseixfloatampe
	float tuatara;//xinkuat;iguanaxdellexpaludixax3xocchi,.
	float ankila;//xdinosauroxconxseixfloatampexextrexocchi;similexadxunxgigantescoxarmadillo.
	float kelenke;
	float shrako;//xuccelloxgigantexassassinoxx
	float phoro;
	float trichoto;
	float tapiron;
	float mirminko;//xscimmianxformichiere
	float trick;//xlontra
	float glypto;//xglitptodontexdixpw
	float maimo;//xscimmioaxxaseixarti.
	float tenonte;
	float cothero;
	float thyla;//xmastinoxgigantex
	float kentas;//xcentaurixselvaggix
	float rhinox;//xrinocernetexcarpaxarmadilloxmontanoù
	float sarko;//xorsoxbradipoxgigante.x
	float orrex;//striga xbufaloxrinocenrontexgrifloatfloatlyx
	float zorrag;
	float anthrako;//xippopotamo
	float megisto;//xpuma
	float xune;//xnexux
	//xpopolafloationix
	float talamos;
	float efesto;
	float ermete;
	float vestale;
	float eracle;
	float alake;
	float ilita;
	float aris;
	float paneo;
	float clapo;
	float demo;
	float ipneo;
	float lipare;
	float kraso;
	float erimante;
	float ghoro;
	float khron;
	float fezo;
	float artemis;
	float parna;
	float ura;
	float tarak;
	//xmagicalxcreature
	float oracma;
	float larus;
	float titan;
	float cyclop;
	float trioculus;
	float lestrigon;
	float pirkagia;//xsputafuoco
	float gerion;
	float lerna;// l'idra 
	float ippogrifo;
	float sfinx;
	float esperide;
	float nereide;
	float pegasus;
	float lione;
	float fatuo;//xfuocoxfatuox
	float tartaro;
	float bongol;//xgolemxd'ossax
	float typhoon;//xdragoxtifone
	float tefrago;//xprogeniexdixcenere
	float leuca;//xninfaxalberoxsacra
	float scilla;
	float cariddi;
	float stinfalo;//shrakoxdixferrox
	float pleiade;
	float voro;
	float ogigia;
	float deodon;//xcinghialexgigante
	float medea;//xstrega
	float amorpho;
	float phantas;
	float ortro;
	float tavro;
	float chiron;
	float methila;
	float elafe;
	float aracne;
	float eleade;
	float cria;
	float cerberus;
	float kalopolo;
	float colosso;
	float gegon;//xmostroxdixpietraxdelxsottosuolo
	float vrykola;//xcentauroxvampiro
	float fasma;//xcentauroxspettrox
	float ipnofago;//xmangiasonno
	float psicofago;//xmangiamente
	float necrofago;//xmangiaxmorte
	float amaxon;//xamafloatfloatone
	float narciso;
	float ioto;
	float europe;
	float tizio;//xbrigantix
	float callisto;
	float olico;//xsacroxcotheroxdixdiomede.x
	//xgodnessx
	float orlan;
	float aries;
	//xtoro:xvedixtavro
	float gemine;
	float cancer;
	 //xleone;vedixlione;
	float virgo;
	float libra;
	//xscorpionexvedixscorpionixgiganti
	//xsagittarioxvedixchirone
	float amaltea;//xcapricorno
	float aquarius;//xganimedex
	// IDRUS
	float idrus; 
	// animals
	float squedo; 
	float eondra; // squedo natante
	float dinia; 
	float parvanca;
	float tribrachio;
	float windermer; // squedo strisicante
	float troua; // spugna
	// meduse
	float ciatide;  
	float thalloca; // corallo
	float norcia; // anemone gigante
	float graptolite; 
	float edemsa; // caravella portoghese 
	float bobbit; // verme lombrico con mandicole taglienti
	float funisia; // sanguisuga
	float serpula; // polichete; sessile
	float ottoia; // conchiglia 
	float huitre; // ostriche
	float segra; // maiale di mare 
	float lumacon; // lumaca : tipi bellerofa; cardioa; emiola; escarga; omphalo
	// gasteropodi
	float ammonite; 
	float octocele; // polpo
	float nautocta; 
	// artropodi millepied
	float aysha; // marino; fondale 
	float mulion; //natante 
	float miriak; //millepiedi tessitore talpa gigante.
	// trilobiti
	float egiroka; // lobas; limulo; floatifosuro. // anomalocaride
	// crostacei
	float creva; // gamberetto
	float tetron;// granchio
	float herpa;  // paguro
	float homarda; // aragosta
	// granchi ragno \ 
	float argyro; // rana
	float mismen; // mimetco
	float marato; // saltatore 
	float mirmefloat; // formigca
	float smaragdo; // legionario ù
	// scorpioni d'acqua
	float euripteride; // marino; missote; abissale; pterigoto; fluviale.
	float dapha; // pulce d'acqua
 	float cochon; // onisco
	float barnaclo; // balano
	// echinodermi 
	float astrospa; // stella marina scalatrice
	float blastoide; // vario 
	float eutoid; // terrestre dei ghiacci
	float lucoide; // stella di luce
	float edriosta; // stella tentacolo.
	float furcaster; // ofiuridi: pentremita; tholla.
	float boucla; // riccio sfera con aculei retrattili 
	float echinosferide; // dei ghiacci
	float oursin; // lancia aculei. 
	float slimer; // spazzino
	float eterosta; // gigli di mare; sanguisuga etc. 
	float tunica; // ascidia
	float pica; // anfiosso
	// pesci
	// ostracodermi 
	float agna; // senza mandibola 
	float alesipo; // pterittoide
	float botrio; // pesce corazzato 
	float cauche; // dinictis
	float drepana; // razza ostraca
	float elicoprion; // anguilla
	// cartilaginei 
	float edesto; // manta; mants gigante; squalo elefante.
	float elasmo; // squalo del mare; eugero; squalo costiero;. 
	float gemua; // razza mutante
	float iboda; // pesce pila. 
	float ornitoprione; // luccio
	float requien; // martello
	// ossei
	float baieso; // chicco di riso
	float bulambo;// black moon necrofago 
	float fleura; // pesce fiore
	float mola; // pesce sole \ luna
	float morminco; // pesce elefante
	float muletta; // triglia
	float pandaso; // mastino;storione; gattuccio; con grosse guance
	// può andar fuori dall'acqua
	float pilana;// pesce generico marino  
	float penuko; // pesce pinguino
	float pike; // luccio
	float pikume; // barbo pesce generico d'acqua dolce 
	float pikurla; // pesce palla
	float punana; // tonno
	float pusko; // scorpione
	float volaro; // pesce rondine stensiella pesci volanti 
	float rinopto; // narvalo. 
	// trachino; leccia
	float vekane; // pesce vipera
	float vuku;//  pesce diavolo
	// pesci oggetto
	float amofe; // pesce amorfo; 
	float ikeia; //pesce sega; utensile
	float kaha; // pesce pala; minerario; litorio.
	float kahiai; // pesce seagrub
	float kahippo; // ippocampo 
	float kaia; // pesce rame elettrico
	float kaiaia; // pesce ascia; 
	float kaiame; // pesce unicorno \ chirurgo
	float kehu; // pesce chiave
	float kenton; // pesce scatola \ cassaforte
	float pakeda; // pesce zaino \ marsupio
	float kodhok; // rana psscatricie. 
	float pono; // domestico
	float pukelo; // pesce granchio
	float puokto; // pesce polpo
	float qapu; // pesce vela. 
	float pleco; // ventosa
	float renuko; // pulitore; 
	float oroko; // pesce gatto spazzino mangiarifiuti 
	float rukua; // pesce pietra militare 
	float tukea; // pesce mistico
	float yaya; // pesce sybil
	// cetacei
	float torpilla; // torpedine
	float wakapa; // balena
	float daufo; // delfino
	// pesci mistici
	float crapsico; // psichico
	float phantome; // pesce fantasma
	float cotto; // pesce coltello.
	float aptera; // piranha! 
	// pesci abissali
	float nageo; // pesce scafandro.
	float craptera; // kodhok malvagio abissale.;
	float mendako; // polpo dumbo.
	float harong; // re delle aringhe
	float voctok; // vampiroctele 
	// nautocta gigante
	float goreq; // squalo goblin
	// pesci stega
	float dipnoo;
	float oloptico; // trosa; eustena; oloptico.
	float latimera; // celacanto
	float mutrino; // pesce ornitorinco
	float ripidario; // perioftalmo
	float stega; // ittiostega
	float axolotl; // tritone albino
	float cacope; // drago rana
	float camlia; // camaleonte leone.
	float elipe; // orso luopo anfibio.
	// estemmo; vedi kotor 
	float iguanuga; // iguana tartaruga
	float lepidon; // lucertola con tetsta a boomerang.
	float koakoa; // anaconda
	float mosope; // ippopotamo
	float nexuro; // salamandra gigante parlante
	// rettili 
	float simora; // salamandra lucertola 
	// diploca: vedi lepidon
	float doser; // serpente murena
	float labida; // lucertola iguna
	float liken; // scimmia lucertola
	float mesosa; // mesosauro;
	float nostrago; // lucertola elefante
	float pareia; // tartaruga
	float scutosa; // scutosauro
	float sphenaco; // sfenacodonte
	float vipion;// // vipione. 
	// mistical e magical 
	float zaratan; 
	float carcino; 
	float ozena; 
	float kutor; 
	float haphgur; 
	float thanork;
	float ishtrik;
	float quinota; 
	float jomungo; 
	float bozumo; 
	float mellorg; 
	float yuko; 
	float hyppogra; 
	float phroth; 
	float shuruth; 
	// peoples
	float ragan; 
	float meron; // viola: cernie anfibie. droga. 
	float nauga; // blu: iguane nettuno ; minaccia dell’inquinamento marino. 
	// popolo di posus
	float maquere; // azzurri: sgombri \ scorfani  urano  minaccia della dimensione
	float remu; // bianchi: merluzzi \ pesci gatto saturno  ; minaccia del tempo
	float raina; // verdi:  ile  venere  minaccia della natura 
	float kaila; // bruni: tartarughe torpedine  mercurio  minaccia degli stati
	float rouger; // rossi: pesce scorpione marte ; minaccia del conflitto
	float jauna; // gialli serpentiformi tipo serpentrefloat di pw. sole minaccia dell’energia 
	float lyerh; // lyerh: mante pesci abissali; \ pesce vipera plutone  minaccia della morte. 
	// godness
	float posus; 
	// icarus
	// animals
	float icarus; 
	float plaguer; // batterio gigante
	float plesen; // aeromuffa
	float letayo; // fungo volante 
	float vodorov; // alga nastrescente 
	float moskisk; // superalga 
	float lishay;
	float morskoia; // pennatule
	float svetov; // fiore volante
	float cherva; // verme \ mollusco terrestre 
	float gusen; // verme articolato gigante
	float planor; // medusa assassina volante planoriofloatile 
	float nubiloide; // verme nuvola gigante mangia città 
	float oblako; // nuvola vivente 
	float osmino; // polpo alato \ polipoidi
	float midiakka;  //lamellocefali \ mollusco marino 
	float dushom; // 	mongolfieridi
	float naseko; // insetto icarese artridi 
	float yesmer; // mostro millepiedi mangiacadaveri millepiede serpente 
	float babochka; // insetto alato 
	float bapaho; // esasauro
	float yasker; // teatrasauro 
	float kryla; // saurottero - uccello rettile pipistrello pterodattilo
	float dargoon; // dragone 
	float lomoor; // lomorapti 
	float probon; // probonte
	float ptisa; // uccello generico
	// riunirli in categorie 
	float skonna; // esseri primordiali 
	float celophe; // celophiside
	float herrer; // herrerasauroo
	float rincosa; // rincosauro; mostro psichico boss 
	float protero; // mangiatore di insetti proterosuchus
	float uzha; // mostro simil dinosauro del triassico
	float melano; // mammifero primitivo icarese (su icarus i mammiferi non hanno successo evolutivo)(
	float cinognato; // terapside
	float nayabron; // corazza primordiale.// rettioli corazzati - terra 
 	float anchiso; // anchisauro - tipo brontosauro
	float gherron; // gerrontorace - diploauclo idrese
	float protosa; // coccodrillo lucertola
	float mifloatosa; // dinosauro mostrosuo chimera mifloatosauro . è il predatore inconstratsato degli oceani e delle terre icaresi
	float mastospe; // massospondilo
	float placodo; // placodonte. tartararuga icarese
	float tekodo; // mostro sismico. tecodontosauro
	float vozda; // mostri marini primordiali 
	float askepto; // askeptosauro - mostro marino 
	float dianoga; // mostro tipo shadow of empire ch einfesta le fogne
	float lystrosa; // dinosauro lucertola anfibia. lystrosauro
	float mifloato; // ittiosauro con coda da anguilla e becco da storione
	float notosa; // mostro degli oceani; notosauro. rettile marino fluviale
	float plateo; // plateosauro; dinosauro sferico dei ghiacci
	float floatifofloat; // floatifosuro - limulo; mostro acquatico gigante. ben più pericoloso di quelli di vorkan
	float pervoza; // uccello primordiale . 
	float eudimor; // eudirmorfodonde; pterodattilo elementale - tipo mephit di ded
	float kuhen; // kuehneosauro - tipo icarosauro; lucertola alata dei boschi con ali costola
	float peten; // rettile volante peteinosauro
	float podopto; // lucertola alata come icarosauro
	float tosuko; // coccodrillo uccello ornitosuchus 
	float steklo; // rettili di cristallo. 
	float clevo; // clevosauro ; montagne; terre di cristallo
	float etereo; // rettile vitreo heterodontosauro
	float kristall; // cristallo vivente
	float kristakker; // mostro di cristallo 2 rancor ai  
	float ruzion; // mostro delle pianure di vetro di sidrosforo. rutiodonte
	//  mostri delle terre mobili;
	// fatte di metallo liquido; sidrosforo;
	// rodoglass; cristallo; sidrosforo; vulcani gelati; terre di metallo; terre radiattive
	float skaar; // esaformi (uccelli con mani) avi dei falkor  
	// terre sospese; terre nuvola del grande vortice polare
	float lesotho; // dinosauro acrobatico
	float trilofo; // trilofosauro; mostro trifido scalatore
	float ziviz; // mostro della terra viva
	float bordal; // mostro della terra di cristallo
	float rozen; // mostro della terra di vetro di sidrosforo
	float plazer; // mostro di sidrosforo 
	float zidror; //mostro di sidrosforo 
	// peoples
	float gallas; // falkor armeni;
	float irish; // falkor del paradiso; arcobaleno. equatoriali ed esotici 
	float oriana; // falkor scarlatti cavalcadragh; guerrieri; baschiri.
	float dilan; // delle nevi; gufo pinguino sterna artica; ucraini di città di cristallo
	float crigg; // falkor silvano pappagallo aramaco; lappone
	float vulpus; // falkor pavone \ martin pescatore. acquatici. ciuvaschi
	float konda; // falkor del deserto; rapaci; giallinacei; \ mongoli - tartari
	float skree; // falkor neri delle grotte. gufi; corvi neri. ceceni
	float anakra; // falkor anatra \ paperi lol (boothand duck not disney stuff!) 
	// - le creauture elementali di icarus sono uccelli che usano il potere degli elementi storm
	float sferzer; //s1 attacchi termici; elettromagnetici. disgregazione; radiattivo. 
	float raden; // s2danno da luce; materia; esplosivo.  elevata rigeneraizone della mana. absorb. 
	float acren; // s3questo tipo di creeature elementali storm son dotate di continua rigeneraizone della mana.
	// danni elementali di ogni genere incrementati. invulnerabili dai danni mistici.
	float therzen; // s4 creatura elettromagnetica; danni di luce; fulmine; radianza; radiazioni; ionico.
	// sono letali sui robot e sulle macchine. usano therzio e negaprinio 
	float parzen; // s5 queste creature sono in grado di disgregare le vittime.
	float zenpa; // s6 questi esser usano il parzio beta; posson aumentare a dismisura di dimensone quando muoiono
	// o miniaturizzare le vittime. coloro che uccidon per miniaturizzazioe sono disgregati.;
	// le vittime uccise per ingrandimento sono mutate in buchi neri.
	float ufen; // s7 creature notturne affini al buio; alla morte; e al gelo che vivon nelle terre di cristallo. 
	// son immuni ai danni da luce; laser; elettromagnetici; tenebra e radiazione; acqua; gelo; magia nera.
	// posson rendersi invisibili abilità di cloacking. 
	float negapren; // s8 categorie di creature elettriche terribli e devastanti che lanciano attacchi aoe
	// di folgore; energia e tempesta assolutamente letali. volano per levitaizone
	// magnetica. sono anche in grado di generar uragani e tempeste di fulmini.
	float puller; // s9 creature in grado di alterar la massa dei corpi. posson disgregare le vittime ; tramutarle in tachioni
	// e spedirle nell'iperspazio. son in grado di comunicar istantameneamente fra di loro per mezzo di emissione
	// di ultraonde e hanno una mente collettiva. 
	float acrimer; // s10 infliggon danni di tempo; animspeedscale e movespeedscale assurdamnte veloci. fp_speed a 5
	// sono esseri temporali in grado di spostarsi fra passato presente e futuro e di storcer il tempo. 
	float camber; // s11 alterano la roccia e la gravità; infliggon danni astrali e son dotati di telecinesi. 
	// posson generar wormhole con cui teletrasportarsi da una parte all'altra del pianeta. 
	float breker; // s12 sono esseri kamikaze affini ai cultist destroyer che esplodono emettendo devasanti quantità di antimateria
	// e di energia esplosiva. sono pericolosissimi e uno di loro può far esploder una intera città. 
	float cafer; // s13 questi essei sono di metallo liquido superfludio e dimorano nel mantello del pianeta.
	// posson assujmer ogni forma e ibnfliggon dannin da metallo; sidrosfrro; elettricitrà. 
	// hanno effetti simli a isterio; entrofio; tetropio; antibio; alvidio e ncrebio. 
	// poteri: polverizzazione delle vittime; danno da veleno immenso.
	// emissione di vapori invecchianti che innetsano processi di necrosi e decomposizione. 
	// cotrollon di non morti di metallo liquido senza anima. 
	// son immuni ad ogni danno; deboli soloo agli elementi sacri! 
	float ruber; // s14 questi esseri non sono influenzati dalla gravità pertanto possono volare liberamnete.
	// sono creature di cristallo fatte di rubanio o vetro. hanno devastanti poteri 
	// telecinetici.  
	float effimer; // gli esseri di tale tipo posson volar ad altissime quote cadendo verso l'alto. 
	// son aerodinamici ed estremanmente veloci oltre che eterei e gassosi.
	float vozduch; // creature di pura aria; velocissime. 
	float clamer; // creature resistenti ai veleni fisici e spirituali e al vampirismo. 
	// cacciate per ricavarne droga.
	float koller; // queste creature son in grado di combinar fra di loro attacchi elementali
	// multipli colpendo con elementi ed energie ibride in modo randomico. 
	// uccisi droppano kollium.
	float catmer; // esseri in grado di generare vortici e tempeste di sabbia e cristallo.
	float phocrer; // esseri che infliggon danno termico; resistentissimi a calore e fuoco. 
	// gli uccelli di questo tipo sono tipo calcid the flame di pw.
	float vecher; // strida e attacchi sonici come gli howler. son uccelli sonici corridori.
	float temny; // questi uccelli son in grado di rendersi invisibili e attaccan con magie di tenebra.
	float wozzer; // creature che usano il potere di volterio; elettrolitio; relettrio; morio; cuzerio
	// e aurio. infliggon danno di luce; sacro; fulmine; radianza; metallo e son invincibli alla magia.
	// dimoranon nei cumulonembi e nelle nubi tempestose
	float sthroma; // questi esseri usano andio; corio; relettrio e irio.
	// danno da vento; pioggia; tempesta; gas; vuoto; pressione. molto agili e veloci. 
	// sembrano fatti di nuvole. quesrti elementi son usati anche dai rettili volanti 
	// del pianeta 
	float xiller; // omuncolo di gelo e cristallo in grado di abbassar le temperature a valori
	// prosismi allo zero assoluto e di evocar blizzard. estremanente pericooso; immune a fuoco e gelo.
	// usa allopirio come sostanza principale. 
	// strenio e tenterio: son usati dai rettili corazzati del pianeta. 
	// danno di terra; immuni alla terra; fp_protect a 3. possonn esser anche immuni ai danni fisici.
	float zirek; // questi esseri sono simili alle donne di metallo di lothranis e posson lanciar
	// attacchi di energia elementale aoe estremanente potenti. 
	float seeker; // queste creature usano il potere del baralio e hanno poteri psichici fp_sense a 3. però son sorde.
	float kronoser; // i kronoser sono pericolosi elementali di t	empo. vedi acrimio. 
	// uccisi droppano zaffiri di kronisio. 
	float freden; // i freden sono esseri che disgregano gli oggetti e infliggon danni da radiazione; molto pericolosi. 
	float radeon; // radeo questi esseri brillano come soli. immuni alla luce; se colpiti da attacchib di luce li acculano al loro interno per poi riemeterli sotto forma di aoe.
	// son affini alla luce come potere.
	float crayer; // esseri che attirano enormi quantità d'aria e che le usano contro i nemici. 
	// hanon totale dominio sull'aria. i crayer vivon nell'occhio degli immensi uragani del pianeta. 
	float iber; // questi esseri son in grado di infondersi di magia multielementale e di infligger danni random
	// di luce; buio; sacro; veleno; vita; morte; fuoco; terra; aria; acqua; gelo e calore; spazio e tempo. 
	// questi esseri son anche di metallo liquido deformabile a memoria di forma.;
	float ofloatene; // cristallo di ofloatirio senzientie. è un temibile essere gassoso dal nucleo nero e lucente; 
	// in grado di strappar l'ossigeno dalle vittime e di risucchiarlo in sé.
	float menser; // esseri rarissimi con abilità simili agli shamoriti dei cranion; intriis di
	// poteri telepatici devasanti. 
	// abbinali elementi charm e storm agli uccelli icaresi.  ogni specie ne detien un potere.
	//tinamide;
	float straus; // struzzo
	float rheida; //  rheiformes
    float wiki; // kiwi
	float casuar; // casuario
	float lebed; //  cigno; 
	float petuk; // galliformi.   megapodidi; numidi; quaglie; fagiani
	float gavin; //  gaviforme paperi; duck etc. 
	float sfenifloat; // pinguino
	float procella; // mariniprocellari.  diomedeidae; procellariidae; hydrobatidae; oceanitidae
	float svassus; // svasso 
	float pheton; //  phaethontiformes   phaethontidae  phoenicopteriformes    phoenicopteridae
	float cycon; // cicogna   ciconiiformes     ciconiidae
	float peleca; // pellicano   pelecaniformes
	// suliforme  
    float thresk; // uccello non morto cafornio 
 	float ardeide; // uccello di piertra - cambrio 
	float scopia; //uccello scorpione 
	float balaen; // uccello balena. esplosivo 
	// threskiornithidae; ardeidae; scopidae; balaenicipitidae; pelecanidae
	float phrega; //  sulidae; phalacrocoracidae; anhingidae; fregatidae
	// mostri elementali 
	float cathar; // avvoltoi 
	float sagittar; // serpentari 
	float pandion; // pandion
	float orel; // aquila 
	//  accipitriformes
    //  cathartidae; sagittariidae; pandionidae; accipitridae
	float falcon; // rapaci     falconiformes     falconidae
	float otide; //   otidiformes      otididae
	float magasca; // mesitornithiformes     mesitornithidae . madagascar
	float cariam; // cariamide 
	float pygifo; //  eurypygiformes eurypygidae; rhynochetidae
	// uccelli con corna e sputafuoco \ kagu e tarabuso 
	float gru; //  gruiformes
    // helior; // uccello del sole.
	// rallis; // rallidae; 
	//phosphor; // uccello del psophiidae; gruidae; 
	// aramid; // aramidae
	float chayka;  // gabbiano 
    // turnicidae; burhinidae; chionidae; pluvianellidae; haematopodidae; dromadidae; ibidorhynchidae; recurvirostridae; charadriidae; pluvianidae; rostratulidae; jacanidae; pedionomidae; thinocoridae; scolopacidae; glareolidae; laridae; stercorariidae; alcidae
	float pterocle; //  pterocliformes  pteroclidae
	float golub; //   columbiformes   columbidae 
	float popuga; // pappagalli 
	float hoatzin; //  opisthocomiformes     opisthocomidae
	float musofago; //  musophagiformes     musophagidae uccello alfeico
	float kokoo; //  cuculiformes       cuculidae ufonio 
	float fillin; //  gufo ufonio 
	float caprimul; // nittibi; guaciari
	float apus; // uccelli insetto senza piedi; come i colibrì. uccelli impollinatori
	float colide; //  coliiformes
    float trogol; // trogoniforme 
	float curol; //   leptosomiformes uccello guaritore
	float corace; // coraciiformes
	float vorona; // corvo ufonio 
	float dyatel; // picchio. piciforme galbulidae; bucconidae; capitonidae; semnornithidae; ramphastidae; megalaimidae; lybiidae; indicatoridae; picidae
	float vorobe; // passero*/
	// creatures mistich demoniache falkor  - oppopsti agli dei 
	float kimer; // chimaera - fuoco 
	float wiwer; // viverna - cristallo liquido
	float nephele; // nefele - tempesta 
	float floatolotl; // mostro simile a quetzalcoatl
	float hek;  // ehecatl; spirito del vento gelido e distruttore
	float garpya; // arpia; mostro della terra. 
	// godness
	float orian;// fuoco \ sidrosforo 
	float vulpur; // mare 
	float craig; // aria 
	float xilog; // gelo 
	float scriek; // terra
	float gal; // luce 
	// CRYUS
	// germi
	float criofagus; 
	float criococcus; 
	float criea; 
	float criozoo;
	// piante
	float prismer; // prismalghe
	float kristala; // cristallidi
	float baraphula; // fiore di ghiaccio 
	float gamsha;  // crioerbe
	float rukha; // criofloatili (alberi di cristallo)
	float cyau; // funghi: cloromiceti; feomiceti; rodomiceti
	float criusa; // medusa adiposa 
	float criorme; // verme adiposo
	float lola;	// ammoniti dei ghiacci  
	float aktopa; // polpo dei ghiacci 
	float cipleka; // criolumache
	float kamala; // miriofloat
	// gigachite tipo obaminie lumaca con conchiglie dorsali 
	// artonfloat; artropodi 
	float jingha; // aragosta \ scorpione del gelo 
	float makura; // ragno dei ghiacci 
	float kekada; // paguro gelido
	float kriketa; // tetronfloat simile a grillo corridore 
	float kamila; // formica; mantide; insetto in generale 
	float macha; //  pesce 
	float sarka; // pesce a squame silicee 
	float chapa; // anfibio foca (fan girini lol ) 
	float sahan; // orsidi
	float mathi; // criotopi 
	float caran; // uccello sferottere 
	float weizerfloat; // uccelli pescatori
	float kerfloat; // uccleli rapaci kerfloat  rapaci
	float bhamger;  // fruifloat - mangiano painte di cristallo
	float thala; // ittiosferidi manta 
	float hvela; // ceton 
	float dalphin; // ittion 
	float sarpa; // ofidoidi
	float ghumrek; // echinoidi
	float kanaga; // saltoidi
	float ekro; // polpoidi
	float bamda; //  antropoidi
	float guphar; // gufaruko; speleolidi (ragni) 
	float tila; //	litosferidi (talpe)
	float syala; // necrosferidi (sciacalli)
	float barnara; // termosferidi
	float botafloat; // fitosferidi
	float katilom; // razosferidi falciatori corazzati con lingue a frusta occhi composti zanne da vampiro
	// e schiena ricoperta da acueli
	// etnic group 
	float sferfloat; 
	float sferon;
	float neron; 
	// creature elementali \ divinità 
	// ogni divinità ha dei mostri elemental che usan la sua stessa classe.
	float grin; // dio cometa che viaggia fra i mondi a portar la vita per panspermia
	float parsi; // ginfa spiritio custodi di grin - ognuna è legata a simbolo atavico
	float varsa; // pioggia / acqua 
	float hium; // neve; gelo; freddo. 
	float surya; // sole
	float nyaya;	// grandine; dea della giustizia
	float nilago; // metano - fuoco azzurro 
	float vijaya; // luce - vittoria 
	float udana; 	// vento; ninfa alata 
	float candra; // luna-buio-grotta (buono) 
	// malvagi 
	float ciso; // gelo (malvagio) 
	float gupha; // luna-buio grotta (malvagio) 
	float balna; // fuoco ardente malefico. 
	float amdhi; // spirito delle bufere 
	// karsus
	// animals
	float shuy; // bubbone gigante; peste bacteria
	float blastoderma; //mostro infettante della peste
	float sheng; // lattugoide. 
	float tai; // muschioidi
	float zaol; // alga algomiceti
	float hua; // floromiceti
	float cao; // era
	float zhongshi; // erba fida 
	float shu; //	necralberi
	float pofuda; // saccotelminti
	float yongba; // nototelminti
	float melyur; // anoculopiscidi - pesci senza occhi 
	float qinwa; // radaranuri
	float floatiyi; // lucertola drago sotterranea.
	float juzhi; // talpoide 
	float blanf; // chiroide pippistrelli radar
	float polpolk; // yuhak
	float haizhe; // mostro medusafluttante delle cavenre 
	float ruko; // 	necroloom
	float florolk;  // gfloatun florolk
	// artridi
	float minia; // trivelleoide; trismulloidi
	float zanbu; // trilobiti
	float xiezi; // eureride; - aragoste granchi scorpioni d'acqua
	float xie; // granchio 
	float kunchon; // insettoidi- bigbug
	float ytok; //  simili a coccinelle 
	float formix; // formifloat formiche giganti 
	float cicax; // cikafloat  grilli cicala
	float manx; // manfloat tipo mantidi grillo 
	float splax; // splafloat scarabei
	float lymel; // lymel: libellule e api 
	float zarz; // zarz; zanzare
	float scrab; // scarabei 
	// peoples
	float antwer; // razza insettoide
	float arcadian; // arcadico
	float zoff; // zoffese
	float formigian; // formigian 
	// magical elemental monster creatures
	float buleo; // tardigrado del vuoto creatura malefica. 
	float zenbuk; // verme abissale del pianeta immune a metallo; calore e lava.
	// divinità arcadica
	float huzin; // verme del deserto adorato dai zoffesi 
	float tamenza; // coleottero dei ghiacci; divinità formigiana
	// legendary japan creature mithology
	// godness comuni a tutti i popoli  
	float an;  // la volta; il grembo; il "cielo" ultraterreno; lo spazio.
	float ki; // terra; considerato come tutta la speleosfera. 
	float enil; // aria 
	float enki; // acqua 
	float anunna; // assemblea divina; gruppo di dei.
	// divinità zoffesi 
	float anki; // asteroide; personificato nelle 3 lune del pianeta. 
	float aruru; //il principio creatore.; 
	float ashnan; // la dea custode dell'erba fida. 
	float ninz; // bau / ninisinna /gula (dea della medicina) 
	float bel; // belili (dea della luce; consorte del dio solare bel) 
	float meku; // dimpemekug (dio scriba degli inferi) 
	float zibzu;  // dumu-zi-abzu (dio della vegetazione ) 
	float gilgam; // eroe divino antwer enkidu gilgamesh (mitico re di uruk; protagonista dell'epopea di gilgamesh) 
	// haia (sposo di nidaba) 
	float eresh; // la dea del buio sotterraneo ched tutto avvolte. 
	float gesthin; // la dea dello stige che da la vita col suo lago. geshtinanna 
	float kur; // malvagio dio del sottomonto
	float inanh; //inanna; dea madre. 
	float ish; // ishkur - governa il clima nella speleosfera. 
	float izthar; // adorato dagli arcadici; dirime le divergenze e porta la pac.e 
	float lahar; // custode delle bestie e del bestiame.
	float nammu; // il mare primordiale; l'essenza stessa dello stige
	float nanna; // (dio della luna) 
	float nida; // nidaba (dea della saggezza; della scrittura e della letteratura) 
	float ninazu; // (dea degli inferi) 
	float ninlil; // (dea dell'aria e del grano) 
	float ningal; // (madre di utu e dea della luna) 
	float utu; // dio della luce. 
	float ziuz; // ziusudra; il primo antwer; dai cui 3 figli derivaron i popoli antwer attualim che si salvò dal "diluvio"
	// causato dall'allagamento di tutta la speleosfera ad opera dello stige. 
	float humbaba; // guardiano del giaridno dell'aldilà;
	float izhum; // ishum; dio del fuoco
	float keret; // secondo eroe ; signore dei ragni 
	float lamazu; // terzo eroe; signore delle mantidi
	float nizrok; // dio della pietra
	float nusku; // signore delle lucciole fuoco e luce; o meglio delle creature che brillano nel buio. 
	// spiriti arcadici - divinità indu
	float zhramak; // bhrama 
	float veda;  // saggi ancestrali depositari del sapere ancestrale cosmico
	float vushna; // visnu? 
	float kriznak; // krisha
	// spoiriti formigiani 
	float izan; // divinità gemelle formigiane creatrici dei primi antwer. 
	float yaza; // gli 8 antwer ancestrali; da cui ebbero origine tutte le tipologie
	// della loro specie e tutte le creature insettoidi del loro mondo. 
	//( awazi iyo (shikoku) ogi tzuku iki tzuzi (tsushima)
	// zad; yamato (successivamente honshu)
	// demoni antwer che vivon nelle oscure terre dell'antartico
	float yomi; // i signori della morte 
	float kami; // gli oggetti animati. 
	float kappa; // mostri scimmia l'altra razzaz del pianeta; qua poco più che belve ferali
	// accerrimi nemici di tutti gli antwer; imprigionati nel gelido sud
	float kitzun; // l'astuta e infide volpe del sud; che comanda un'armata di ratti che spargon
	// ovunque pestilenza e dolore. 
	float nue; // mostro mutaforme con testa da scimmia; corpo da cane procione; zampe di tigre; la coda di serpe 
	// è un ladro infido.
	float ono; // l'orco degli antwer
	float tanuko; // cane procione (può fondersi alle stalattiiti
	//( e alle stalagmiti della speleosfera per nascondersi e passar inosservato.
	// è affine allo stregatto) 
	float wayang; // son i tengu ;i goblin degli antwer
	float tzukumo; // tsukumogami. barattolo da tè animato? boss kami
	float yama; // yamauba; strega
	float yokai; // spettro buono 
	float yataga; // spettro corvo 
	float allip; // omuncoli; pupazzi.
	float yureo; // fantasma reaper; malvagio 
	// tipi di spettri: 
	float jiba; // jiba- jibakurei: spettro di suicida o rimpianto. infesta un luogo.
	float hyor; // hyoirei: uno spettro che si impossessa del corpo di un vivente. 
	float onro; // onryo: spirito vendicativo. torna a perseguitare chi li ha maltrattati 
	float ubu; // ubume : spiriti di madri morte nel dare alla luce un figlio.-
	float goryo; // spirito di aristrocratico morto per intrighi di palazzo o tradimento.
	float funay; // funayurei spiriti di marinai morti nello stige; 
	float zazi; // fantasmi di bambini; dispettosi; poltergeist
	float gaki; // persone morte per i propri vizi. son vampiri ripugnanti perennemente affamati e assetati.
	float jikini; // necrofagi  
	float iki; // ikiryo; // fantasmi di vivi o di persone morenti! l'anima si separa dal corpo di chi si vuol vendicar; per andar a perseguitar il nemico.
	float genpei; // fantasma di samurai; morto in battaglia.
	float banshee; // fantasma seduttore. 
	// erebo 
	// creatures
	float patofitus; 
	float plasmer; 
	float amoeba; 
	float patal; // alghe nere 
	float patom; // patomuschiodi
	float dionea; //dionee 
	float witrees; 
	float wictus; 
	float pumpkin;
	float florotox;  
	float pyrotox; 
	float warm; 
	float tremor; 
	float starler; 
	float snailer; 
	float coppershark; 
 	float moray; //murena
	float manta;  
	float piranha; 
	float seashark; 
	float wayshark; 
	float lisk; //lisca 
	float iguanoid; 
	float saurin; 
	float reptile;
	float triceratop; 
	float stego;
	float rex; 
	float blackdragon;
	float tardragon; 
	float vampat; 
	float hedger; 
	float vermin; // topo 
	float aracnoide; 
	float naker; // millepiede  
	float narpius; // scorpione
	float bumblebee; // vespe giganti
	float scarafus; // scarabeo gigante. 
	float skelide; 
	float skfish; // lisca di pesce 
	float skdiplo; // diplo schelidi
	float skfrog; // rana sceheltro 
	float sklizard; // lucertola d'ossa. 
	float skbird; // skeelder volante 
	float skcow; // skeelder rancor
	float skwolf; // skeleder wampa
	float bonker; // skeelder monkey 
	// skeelder race and etnical groups
	float slobber; // mutanti skeelder; uomini di fango 
	float necrotail; // uomini scorpione tatuati. sole; zefloatas. 
	float barabek; // uomini blatta; klahom
	float morpher; // mutaforma 
	float witcher; // horroria female witch
	float sorcerer; // horroria male wizard 
	float momor; // stregoni mostruosi; signori della magia opposti
	// ai servi di xadroom
	// evil sorcerer. stregoni cinesi vestiti con tuniche alla confucio  nettuno 
	float reticulan; // grigi; grigi tipo kamonoani urano  klahom
	// portan incubi e controllano i manicomi dell'impero.
	float devil; // diavoli! i più terribli avversari degli skeelder 
	// demons of orghaus; categories. 
	float aigipan; // caprini: corna caprine sulla fronte e zampe di capra) 
	float arren;  // privi di narici o privi di nas
	float antipod; // i piedi rivolti all'indietro) 
	float monokle; // (con un solo occhio sulla fronte) 
	float artabafloat; // (quadrupedi) 
	float astom; // (privi di bocca) 
	float blem; // (privi di testa e con la faccia sul ventre) 
	float brachistom; //  labbra e occhi cuciti! 
	float dcyclop; // giganti e con un solo occhio.
	float cynocefalus; // testa di cane
	float fomor; // testa di capra. 
	float gegen; // corna 
	float gorgad; // corpo pelloso. 
	float imante; //  gambe fini e piatte come strisce di cuoio
	float ipporus; // zoccoli da cavallo 
	float nisicaste; // 3-4 occhi
	float panozi; // orecchie enormi.turelim style
	float raab; // rahabim style.;
	float pigmer; // demoni nani
	float sciapod; // con una sola gamba e un grande piede. */
	// skeelder etnical groups 
	float skeelder; // defauult
	float raska; // artici; simili agli unnni con grosse teste a epra
	float anglorgian; // ossati;emaciati; lineamenti scozzesi; inglesi e transilvani; saturno. 
	// freddi; gentili; astuti e ingannatori 
	float swamper; // delle paludi; con capelli cadenti e setosi e aspetto da gollum
	float horrorian;  // ossati con lunghi capelli alla eddie; cagneschi venere. rozzi; ignoranti.
	// zotici; insensibli e crudeli. dei montanare e dei contadini insomma. 
	float zefloatan;//scorpioneschi e tatuati western i più feroci; aggressivi e sanguinari. 
	float klahom;
	float indi; // mohicani e tribali assassini luna. seguaci del pentavirato. 
	// fanno anche sacrifici umani. tatuati e conciati come pellerosse cannibali.
	float kuzeno; // evil genius. grandi teste da cranion poteri psichici limitati 
	float bloodgauzer; // skeelder bloodsucker mutant
	float vod; // 
	float nubilan; // spettri ovlon
	float ruddy; // anime bianche; grige; nere 
	// skeelder godness
	float alvid; 
	// for horroria 
	float hagraven; // arpia
	float basilisk; // basilisco
	float haunt; // erebo cerbero
	float caradrius; // caradrio
	float yale; // 
	float fauno; 
	float lycan; 
	float crocote; // uomo coccodrillo 
	float naga; // uomo serpe 
	float leviatan; 
	float darksfinfloat; 
	float darkvyvern;
	float unicornus; 
	float syren;
	float chupacapra;
	float xamax; // erebus amazzone 
	float chimaera;
	// for zefloatas 
	float annuna; // dei sanguinari primordiali 
	// quetzal; zaund; 	cicactli; cicavateo; 
	// nahual; 	floatolotl; ; // acchiappasogni  
	// anglorgia
	float dacius; 
	float babau;// acchiappasogni 
	// klahom; 
	float frankie; 
	float hitchhiker; // autostoppista fantasma
	float ghostdriver; // pilota di auto fantasma
	float jason; // serial killer 
	float dyman; // alligatore delle fogne
	float choppervyper; 
	float spooner;
	float morgellon;
	// nubila
	float jikin; 
	float hella; 
	float nightmare; 
	float succubus; 
	float puck;
	float jinn;
	float eris; 
	// a questa classe appartengon i seguenti mostri: 
	float moros; 
	float ker;
	float tanath;
	float hypnos; 
	float onerion;
	float momo;
	float oizys; 
	float nemesi;
	float apate; 
	float philote; 
	float geras; 
	float keres;
	float letum;
	float lysymele;
	float epifron;
	float styx; 
	float ate;
	float makhai;
	float ponono;
	float limo;
	float algea;
	float isminar;
	float fonor;
	float androktas;
	float neikea;
	float pseudos; 
	float amfiolo;
	float horkos; 
	// momon island godness
	float primeval; // entita' ancestrali skeelder pre impero 
	float aita;
	float aipan;
	float ani;
	float alpan;
	float aplu; 
	float artume;
	float atunis;
	float charun;
	float culsu;  
	float februus; 
	float laran; 
	float lasa;
	float losna; 
	float mania; 
	float mantus;
	float maris; 
	float northia; 
	float satres; // time streamer
	float selvan;	
	float semia; 
	float sethlan; 
	float suri; 
	float tagehe;
	float taitle; 
	float usil; // black sun
	float vanth; 
	float veive; 
	float vicar; 
	float veltrun; 
	// STARSHIPS AND SPACE CREATURES! 
	// BOATS, // for sea battle fantasy
	float quadrireme; // scialuppa vesu 
	float frisian; // // nave da guerra dei Kos
	float galea;  // nave da guerra media dei vesu
	float galeon; // nave da guerra vesu
	float submarine; // sommergibile in generile / yerkken dei gorkan
	float barge; // chiatta o grossa nave da trasporto; generica.
	float speedboat; // motoscafo; nave veloce a motore 
	float scuba; // batiscafo generico
	float steamboat; // battello a vapore. 
	float vessel; // veliero generico 
	float idrocar; // idroscafo dei cranion 
	float drakar; // nave vichinga nark 
	float playa; // centopulos sottomarino squalo 
	// ragan e falkor: vedi skimmer
	float idrolaz; // navi falkor 
	float idrobob; // for sferfloat
	float junk; // giunca \ nave formigiana 
	float tinker; // petroliera o nave della flotta skeelder 
	// volanti
	float alian; // aliante vesu
	float mintada; // mongolfiera khrolen \ generica 
	float zemba; // aerostato dei monker 
	float hovercar; // vettura dei cranion 
	float joen; // galeoni volanti dei nark!
	float airlaz; // aerei dei falkor 
	float aerobob; // aerei sferfloat 
	float trichopter; // elicottero skeelder - spesso d'assalto
	// class_zeppelin; // dirigibili \ mongolfiere etc. 
	// class_octopus; // as probe droid
	// space battleships
	// class_pod // generic container;
	float sphere; // contiene esplosivi 
	float fueltank; // contiene carburante 
	float silo; // contiene cibo
	float penta; // contiene sostanze organiche.  
	float efloata; // contiene tecnologia. 
	float moric; // contiene sostanze arcane. è come i magazzini vetonga. vedi monker 
	float clessidra; // contiene fluidi 
	float gastank; // contiene gas
	float bell; // contiene rifiuti.;
	float ark; // contiene creature viventi. 
	float diamond; // energetici 
	float frosty; // contiene refrigeeranti 
	float bofloat; // contiene minerali. 
	float rombus; // contiene armi 
	float torus; // costrittore magnetico usato per scagliare e spostar i cargo.
	float canister; // ricevitore magnetico che riceve i cargo. 
	// cargo 
	float tug; // rimorchiatore e riparatore.
	float miner; // rimorchiatore minerario.
	float fregata; // nave da trasporto modulare 
	float container; // nave porta container sopracitati
	float junker; // addetto alla nettezza urbana costmica.
	float swingminer; // piazza mine o le rimuove usato dai militari. 
	// torrette difese e satelliti d'attacco. 
	float pyro; // scaglia obici incandescenti che fondon lo scafo delle navi.
	// usa attacchi incendiari  usa elementi termporei 
	float sunsprider; // lancia dardi di plasma e radianza.usa elementi energei
	float ionic; // lancia impulsi elettromagnetici. usa energei 
	float srhapner; // canonne che lancia granole a frammentazione. usa metalli arcani
	float shipmolder; // lancia tossine velenose che sciolgono gli scafi. 
	// infligge danno da alchemico \ veleno 
	float nymphayer; // disgregatore. è equipaggiato con gli elementi storm.  
	float arborean; // una difesa organica usata dai vegeter come controarea
	// sparge a raggiera semi e gusci spinosi che scaglian attacchi bioorganici
	// di tipo mishmolder o disgregatori. 
	float nuonner; // terrificante cannone che scaglia armi nuoniche. 
	float tblaster; // installazione montata sugli asteroidi cannoni laser. 
	float tseeker;// torrette specializzate nel lanciar siluri
	float zilzal; // cannoni che lanciano bombe e proiettili sismici. 
	float scrumbler; // atk psichici rivolti ai piloti; non ai caccia in se. 
	float sadat; // questa categoria lancia onde soniche e vibrazioni che mandan 
	// in frantumi gli scafi con le loro "urla" 
	float tcryer; // difese che lanciano attacchi congelanti o proietitli fluidi. 
	float moai; // gradius head stones? 
	float nefiler; // scaglian attacchi di elementi atmòs si trovan nelle installazioni
	// delle nebulose. 
	float darkner; // usano elementi oscuri e negromantici. 
	float mooner; // usano armi mistiche lunari e\ o artificali d'aavanguarida 
	float interdictor; // usani armi subspaziali. 
	// truppe di terra 
	float grabber; // skeelder e truppe di invasione 
	float empire; 
	float alliance; // alleati 
	// tipi di hovercraft
	float quagmire; // è in grado di spostarsi sopra laghi di acido; melma; sabbie mobili; paludi
	float sourer; // si sposta sopra acido o fluidi corrosivi.
	float magmer; // è in grado di spostarsi sopra la lava
	float sandcrawler; // tipico dei krholen
	float tarpitter; // veleggia sopra i laghi di catra
	float uro; // veleggia sopra i laghi di catrame di alvid ha forma a cupola
	float zidkler; // usato dai falkor per spostarsi sopra le distese di cristallo
	float sauger; // a forma di ciambella; usata dai falkor per spostarsi sopra il metallo liquido e il sidrosforo. 
	float skimmer; // usato dai ragan sabbia
	float climber; // per mondi rocciosi. 
	float lyan; // vola sopra gli alberi  
	float tractor; // agricolo va su erba e prati   
	float floater; // antigravitaizonale 
	// carri armati e camminatori 
	float kafuker; // yulo \ carro dei vesu. posson esser armati e d'assalto; pieni di kaful. 
	// corron su binari sopra la lava. 
	float tank; // tank laser 
	float babbada; // carro armato dei khrolen. spara obici esplosivi is already as creature for all tanks creature i guess
	// sometime you fight a tank also as humanoid... 
	float vcab; // carri steampunk dei monker 
	float manfloat; // il più diffuso; a forma di insetto; di vario tipo.
	float sapher;// camminatore khrolen simil pterodattilo meccanico
	float robotolk; // camminatore d'assalto khrolen e mech multiarma in generale
	float mech; // mech multiarma: corron sui cingoli e hanno braccia cannoneggianti. 
	float ducker; // camminatore su pattini dei ragan
	float jumper; // robot saltellante tipico degli sferfloat  e dei mondi lunari; difficili da colpire. 
	float tortoide; // corazzato e simile ad una enorme tartaruga;camminatore d'assalto ragan
	// classe usata anche per i falkor 
	// basi
	float dagoon; // base a cupola da cui decollano caccia standard
	float gamuun; // base da cui decollano i caccia flyer skeelder d'elite.
	float flower; // base da cui decollano caccia vegetali \ vegeter etc.
	float kamooon; // base insettoide da cui decollano i caccia antwer
	float buller; // bolla gigante da cui attaccano creature ameboidi.
	float tower; // torre di lancio di caccia; tipica della marina spaziale.
	// comune nei mondi con liquidi mefitici di vario tipo. 
	// camminatori 
	// class_walker; // big armored walker as atst
	float baocha; // camminatore monker
	float elembante; // camminatore d'assalto monker 
	// class_sat; // telecomunication sat. 
	float sat; // default sat
	float tilter; // this asteroid freeze the ships near
	float charmer; // this asteroid invert cmd directions of player if is close! 
	float wavetronik; // genera attacchi aoe di vario tipo a chi si avvicina tropppo
	float efloatplorer; // sonda esplorativa. 
	float spiotronik; // satellite spia
	float biotronik; // satellite di ricerca
	float ultrasat; // comunicazioni iperspaziali; tachiofari. 
	float ipnosat; // satellici ipnotici
	// class_shuttle; // navetta e sonde
	float shallop; // scialuppa militare; fa la spola fra navi e stazioni  
	float pioneer; // navetta di esplorazione
	float searcher; // navetta di scienziati
	float albatros; // navetta passeggeri 
	float yacht; // nave di lusso per viaggi da sogno e crociere nello spazio
	float epod; // guscio di salvataggio 
	// patrolship
	// ostili che trovano. 
	float ird; // pattuglia usata dai khrolen
	float ragal; //  pattuglia usata dai falkor 
	float zakkar; // ispirati ai garrul di gradius . 
	// ricognitore vesu; vortica su se stesso e può speronare le vittime
	float venus;// ricognitore monker. non attacca; esamina e torna indietro
	float mazaar; // ricognitore bombardiere dei centopulos 
	float fose; // ricognitore dei ragan simile ad un trilobite 
	float uska; // usato tipicamene dalla piuma rossa.
	float urgol; // ricognitore \ cannone mobile tipico degli sferfloat; dalla forma a cupola
	float zeb; // mina spaziale spia invibile usata dagli antwer.
	float rush; // satellite usatao dai nashar 
	// caccia
	// fighter typs of my races. 
	float hoverstar; // cranion fighters 
	float fyron;// vesu fighters 
	float knuby; // khrolen fighters 
	float spider; // vegeter robo fighters
	float yver; // monker fighter bomber 
	float ashak; // aper bomber 
	float ancle; // centopulos fighter 
	float trideb; // ragan interceptor 
	float eagle; // falkor fighrers
	float bowler; // sferfloat spy ships
	float stinger; // antwer fighters
	float fly; // skeelder training fighters
	float razor; // raider and smuggler skeelder fighters
	float scrabble; // skeelder escort fighter
	float bomber; // skeelder bombers 
	float vyper; // skeelder military assault fighter 
	float skull; // elite skeelder fighter
	// usati dagli smuggler
	// navi da guerra 
	float avenger; // vesu - simile ad un falco da guerra klingon 
	float sunspray; // khrolen - ispirato alla firespray di star wars 
	float sinflower;// vegeter 
	float ambras; // monker  ispirato ai pynook è un quetzak con occhi in ormio 
	float bomberboat; // centopulos 
	float blastboat; // ragan 
	float preybird;  //  falkor
	float starchaser;// sferfloat
	float swarm; // antwer 
	float armory; // nave di rifonirmneto armi
	float fueltanker; // nave di rifornimento carburante; energia; riparaizone. 
	// skeelder
	// navi d'assalto e vettori  
	float destroyer; // generic distruttore o assault ship
	float kafship; // cannoniera vesu con kafuk spaziali?
	float striker; // khrolen 
	float bulka; // monker 
	float diendor;// nave paralizzante dei cranion  
	float marauder; // ragan 
	float karraka; // dei falkor 
	float neutron; // sferfloat 
	float lancer; // degli antwer 
	float shrouder; // degli skeelder  
	// vettori 
	float vector; // portacaccia skeelder 
	float carrier; // portacaccia dell'unione
	float alvarium; // nave alveare degli antwer  
	float floaterem; // vegeter porta penthas \ spider. 
	float escorter; // nave da scorta e di supporto 
	// incrociatori 
	float moren; // incrociatore che lancia obici di fiamme nere. avenger; // vesu 
	float interceptor; // khrolen
	float gunship; // monker cannoniera 
	float spreader; // vegeter 
	float kosmer; // cranion
	float defender; // centopulos lenta e possente; da difesa-
	float escaper; // ragan cloakshape; // ragan dotata di cloacking; versatile e crea diversivi
	float nebulon; // corvetta dei falkor 
	float ssss;// sferfloat system stealth starship come una sfera borfloat giroscopica gigante
	float lionant; // formicaleone; nave d'assalto degli antwer
	float nightcrawler; // skeelder 
	// stazioni  
	float shiptrader; // commerciale 
	float shipbuilder; // cantiere spaziale
	float array; // stazione di rivelamento. tachiofaro  
	float commrel; // stazione di comunicazione \ tachiofaro 
	float dioneus; // vegeter station organica 
	float shadir; // stazione monker \ aper 
	float minzar; // stazione militare hq cranion
	float enclave; // stazione khrolen; avamposto alleato generico 
	float talon; // piattaforma di difesa - portacaccia
	float lair; // stazioine della piuma rossa 
	float kolumbus; // stazione turistica. 
	float mining; // installazione mineraria asteroidale. 
	float biosphere; // biosfera
	float ovlonor; // stazione fantasma 
	// mine
	float mine; // profloatimity mine 
	float minea; // chaser
	float mineb; // shooter laser \ ionic ionic
	float minec; // laser \ missile switch weapon 
	//class_stargate; // portale
	float stargate; 
	float puzzlegate; // si attiva colpendo le luci nel modo corretto.
	float wormhole; // distorsione spaziotemporale - buco nero 
	float quasar; // quasar! 
	//class_nav; // navigation boa. 
	float oloboa; // olographic boa 
	float astroboa; // boa fisica
	float hyperboa; // boa iperspaziale 
	//class_asteroid; 
	float dustoid; // space dust clouid
	float rockoid; // asteroid of rock
	float goldoid; // asteroid of light 
	float ironoid; // metal black asteroid
	float deltar; // asteroid efloatplosive and radiactive
	float iceoid; // ice asteroid 
	//class_astrolaser; // space cannon defences. 
	//class_biostar; // creature of space. 
	float phoenifloat; // fenici; vivon su mondi lavici.
	float globus; // fatti di plasma; energia e energia e radianza; vivon nelle stelle 
	float yersa; // terribili virus spaziali 
	float starflower; // piante spaziali. hanno come petali vele e pannelli solari
	float starfly; // libellule e insetti spaziali. biologia silicea
	float nairnama; // astronavi viventi 
	float ziffer; // dimensionali; vivon nell'iperspazio; distorcon spazio e tempo.
	float dogon; // creature siderali capaci di manipolare gli elemeneti arcani.
	// son esseri mistici ed elementali 
	float ameboide; // gigantesco unicellulare tipico degli oceani della luna gamma idrese e dei mondi fluidi. 
	float antennoide; // chaos e atomi giganti; questi esseri provengon dalle pulsar son atomi giganti e usano 
	// attacchi di materia storm 
	float cometa; // planetoide gelido senziente che fa attacchi congelanti. si trovano nella periferia del sistema
	float lithos; // asteroidi senzienti che vivon attorno al pianeta karsus
	float hooler; // progenie del vuoto e dell'entropia; vivon nei buchi neri e usano attacchi nuonici
	float orlana; 
// dei cosmici 
	} modifiers;

	struct {
		qboolean canDismember;
		qboolean blowChunks;
	} dismemberment;

};
void LoadMeansOfDamage();
meansOfDamage_t* GetMeansOfDamage(const std::string& ref);
meansOfDamage_t* GetMeansOfDamage(int num);
//meansOfDeath_t* GetMeansOfDamage(const std::string& ref);
//meansOfDeath_t* GetMeansOfDamage(int num);

int GetMeansOfDamageIndex(const std::string& ref);

extern std::vector<meansOfDamage_t> allMeansOfDamage;

