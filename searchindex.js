Search.setIndex({envversion:46,filenames:["background/index","capi/header","capi/index","capi/misc","capi/nvm_addr","capi/nvm_bbt","capi/nvm_be","capi/nvm_bounds","capi/nvm_buf","capi/nvm_cmd","capi/nvm_dev","capi/nvm_geo","capi/nvm_meta","capi/nvm_mode","capi/nvm_pmode","capi/nvm_ret","capi/nvm_vblk","capi/nvm_ver","cli/index","cli/nvm_addr","cli/nvm_bbt","cli/nvm_dev","cli/nvm_env","cli/nvm_lba","cli/nvm_vblk","developer/backends","developer/backends_spdk","developer/index","developer/testing","index","prereqs/index","prereqs/ocssd","prereqs/os","prereqs/tools","quick_start/index","refs/index","tutorial/index","tutorial/nvm_addr","tutorial/nvm_addr_vio","tutorial/nvm_bbt","tutorial/nvm_dev","tutorial/nvm_vblk"],objects:{"":{"1":[4,0,1,"c.1"],"5":[11,0,1,"c.5"],"nvm_addr::blk":[4,0,1,"c.nvm_addr::blk"],"nvm_addr::ch":[4,0,1,"c.nvm_addr::ch"],"nvm_addr::chunk":[4,0,1,"c.nvm_addr::chunk"],"nvm_addr::g":[4,0,1,"c.nvm_addr::g"],"nvm_addr::l":[4,0,1,"c.nvm_addr::l"],"nvm_addr::lun":[4,0,1,"c.nvm_addr::lun"],"nvm_addr::pg":[4,0,1,"c.nvm_addr::pg"],"nvm_addr::pl":[4,0,1,"c.nvm_addr::pl"],"nvm_addr::ppa":[4,0,1,"c.nvm_addr::ppa"],"nvm_addr::pugrp":[4,0,1,"c.nvm_addr::pugrp"],"nvm_addr::punit":[4,0,1,"c.nvm_addr::punit"],"nvm_addr::sec":[4,0,1,"c.nvm_addr::sec"],"nvm_addr::sectr":[4,0,1,"c.nvm_addr::sectr"],"nvm_addr::val":[4,0,1,"c.nvm_addr::val"],"nvm_bbt::addr":[5,0,1,"c.nvm_bbt::addr"],"nvm_bbt::dev":[5,0,1,"c.nvm_bbt::dev"],"nvm_bbt::nbad":[5,0,1,"c.nvm_bbt::nbad"],"nvm_bbt::nblks":[5,0,1,"c.nvm_bbt::nblks"],"nvm_bbt::ndmrk":[5,0,1,"c.nvm_bbt::ndmrk"],"nvm_bbt::ngbad":[5,0,1,"c.nvm_bbt::ngbad"],"nvm_bbt::nhmrk":[5,0,1,"c.nvm_bbt::nhmrk"],"nvm_geo::g":[11,0,1,"c.nvm_geo::g"],"nvm_geo::l":[11,0,1,"c.nvm_geo::l"],"nvm_geo::meta_nbytes":[11,0,1,"c.nvm_geo::meta_nbytes"],"nvm_geo::nblocks":[11,0,1,"c.nvm_geo::nblocks"],"nvm_geo::nbytes":[11,0,1,"c.nvm_geo::nbytes"],"nvm_geo::nbytes_oob":[11,0,1,"c.nvm_geo::nbytes_oob"],"nvm_geo::nchannels":[11,0,1,"c.nvm_geo::nchannels"],"nvm_geo::nchunk":[11,0,1,"c.nvm_geo::nchunk"],"nvm_geo::nluns":[11,0,1,"c.nvm_geo::nluns"],"nvm_geo::npages":[11,0,1,"c.nvm_geo::npages"],"nvm_geo::nplanes":[11,0,1,"c.nvm_geo::nplanes"],"nvm_geo::npugrp":[11,0,1,"c.nvm_geo::npugrp"],"nvm_geo::npunit":[11,0,1,"c.nvm_geo::npunit"],"nvm_geo::nsectors":[11,0,1,"c.nvm_geo::nsectors"],"nvm_geo::nsectr":[11,0,1,"c.nvm_geo::nsectr"],"nvm_geo::page_nbytes":[11,0,1,"c.nvm_geo::page_nbytes"],"nvm_geo::sector_nbytes":[11,0,1,"c.nvm_geo::sector_nbytes"],"nvm_geo::tbytes":[11,0,1,"c.nvm_geo::tbytes"],"nvm_geo::verid":[11,0,1,"c.nvm_geo::verid"],"nvm_ret::result":[15,0,1,"c.nvm_ret::result"],"nvm_ret::status":[15,0,1,"c.nvm_ret::status"],NVM_BBT_BAD:[5,3,1,"c.NVM_BBT_BAD"],NVM_BBT_DMRK:[5,3,1,"c.NVM_BBT_DMRK"],NVM_BBT_FREE:[5,3,1,"c.NVM_BBT_FREE"],NVM_BBT_GBAD:[5,3,1,"c.NVM_BBT_GBAD"],NVM_BBT_HMRK:[5,3,1,"c.NVM_BBT_HMRK"],NVM_BE_ANY:[6,3,1,"c.NVM_BE_ANY"],NVM_BE_IOCTL:[6,3,1,"c.NVM_BE_IOCTL"],NVM_BE_LBA:[6,3,1,"c.NVM_BE_LBA"],NVM_BE_SPDK:[6,3,1,"c.NVM_BE_SPDK"],NVM_BE_SYSFS:[6,3,1,"c.NVM_BE_SYSFS"],NVM_BOUNDS_BLOCK:[7,3,1,"c.NVM_BOUNDS_BLOCK"],NVM_BOUNDS_CHANNEL:[7,3,1,"c.NVM_BOUNDS_CHANNEL"],NVM_BOUNDS_CHUNK:[7,3,1,"c.NVM_BOUNDS_CHUNK"],NVM_BOUNDS_LUN:[7,3,1,"c.NVM_BOUNDS_LUN"],NVM_BOUNDS_PAGE:[7,3,1,"c.NVM_BOUNDS_PAGE"],NVM_BOUNDS_PLANE:[7,3,1,"c.NVM_BOUNDS_PLANE"],NVM_BOUNDS_PUGRP:[7,3,1,"c.NVM_BOUNDS_PUGRP"],NVM_BOUNDS_PUNIT:[7,3,1,"c.NVM_BOUNDS_PUNIT"],NVM_BOUNDS_SECTOR:[7,3,1,"c.NVM_BOUNDS_SECTOR"],NVM_BOUNDS_SECTR:[7,3,1,"c.NVM_BOUNDS_SECTR"],NVM_FLAG_PMODE_DUAL:[14,3,1,"c.NVM_FLAG_PMODE_DUAL"],NVM_FLAG_PMODE_QUAD:[14,3,1,"c.NVM_FLAG_PMODE_QUAD"],NVM_FLAG_PMODE_SNGL:[14,3,1,"c.NVM_FLAG_PMODE_SNGL"],NVM_META_MODE_ALPHA:[12,3,1,"c.NVM_META_MODE_ALPHA"],NVM_META_MODE_CONST:[12,3,1,"c.NVM_META_MODE_CONST"],NVM_META_MODE_NONE:[12,3,1,"c.NVM_META_MODE_NONE"],nvm_addr:[4,4,1,"c.nvm_addr"],nvm_addr_check:[4,2,1,"c.nvm_addr_check"],nvm_addr_dev2gen:[4,2,1,"c.nvm_addr_dev2gen"],nvm_addr_erase:[4,2,1,"c.nvm_addr_erase"],nvm_addr_gen2dev:[4,2,1,"c.nvm_addr_gen2dev"],nvm_addr_gen2lba:[4,2,1,"c.nvm_addr_gen2lba"],nvm_addr_gen2off:[4,2,1,"c.nvm_addr_gen2off"],nvm_addr_lba2gen:[4,2,1,"c.nvm_addr_lba2gen"],nvm_addr_off2gen:[4,2,1,"c.nvm_addr_off2gen"],nvm_addr_pr:[4,2,1,"c.nvm_addr_pr"],nvm_addr_prn:[4,2,1,"c.nvm_addr_prn"],nvm_addr_read:[4,2,1,"c.nvm_addr_read"],nvm_addr_write:[4,2,1,"c.nvm_addr_write"],nvm_bbt:[5,4,1,"c.nvm_bbt"],nvm_bbt_alloc_cp:[5,2,1,"c.nvm_bbt_alloc_cp"],nvm_bbt_flush:[5,2,1,"c.nvm_bbt_flush"],nvm_bbt_flush_all:[5,2,1,"c.nvm_bbt_flush_all"],nvm_bbt_free:[5,2,1,"c.nvm_bbt_free"],nvm_bbt_get:[5,2,1,"c.nvm_bbt_get"],nvm_bbt_mark:[5,2,1,"c.nvm_bbt_mark"],nvm_bbt_pr:[5,2,1,"c.nvm_bbt_pr"],nvm_bbt_set:[5,2,1,"c.nvm_bbt_set"],nvm_bbt_state:[5,1,1,"c.nvm_bbt_state"],nvm_bbt_state_pr:[5,2,1,"c.nvm_bbt_state_pr"],nvm_be_id:[6,1,1,"c.nvm_be_id"],nvm_bounds:[7,1,1,"c.nvm_bounds"],nvm_bounds_pr:[7,2,1,"c.nvm_bounds_pr"],nvm_buf_alloc:[8,2,1,"c.nvm_buf_alloc"],nvm_buf_fill:[8,2,1,"c.nvm_buf_fill"],nvm_buf_from_file:[8,2,1,"c.nvm_buf_from_file"],nvm_buf_pr:[8,2,1,"c.nvm_buf_pr"],nvm_buf_to_file:[8,2,1,"c.nvm_buf_to_file"],nvm_dev:[10,4,1,"c.nvm_dev"],nvm_dev_close:[10,2,1,"c.nvm_dev_close"],nvm_dev_get_bbts_cached:[10,2,1,"c.nvm_dev_get_bbts_cached"],nvm_dev_get_be_id:[10,2,1,"c.nvm_dev_get_be_id"],nvm_dev_get_erase_naddrs_max:[10,2,1,"c.nvm_dev_get_erase_naddrs_max"],nvm_dev_get_geo:[10,2,1,"c.nvm_dev_get_geo"],nvm_dev_get_meta_mode:[10,2,1,"c.nvm_dev_get_meta_mode"],nvm_dev_get_nsid:[10,2,1,"c.nvm_dev_get_nsid"],nvm_dev_get_pmode:[10,2,1,"c.nvm_dev_get_pmode"],nvm_dev_get_read_naddrs_max:[10,2,1,"c.nvm_dev_get_read_naddrs_max"],nvm_dev_get_verid:[10,2,1,"c.nvm_dev_get_verid"],nvm_dev_get_write_naddrs_max:[10,2,1,"c.nvm_dev_get_write_naddrs_max"],nvm_dev_open:[10,2,1,"c.nvm_dev_open"],nvm_dev_openf:[10,2,1,"c.nvm_dev_openf"],nvm_dev_pr:[10,2,1,"c.nvm_dev_pr"],nvm_dev_set_bbts_cached:[10,2,1,"c.nvm_dev_set_bbts_cached"],nvm_dev_set_erase_naddrs_max:[10,2,1,"c.nvm_dev_set_erase_naddrs_max"],nvm_dev_set_meta_mode:[10,2,1,"c.nvm_dev_set_meta_mode"],nvm_dev_set_pmode:[10,2,1,"c.nvm_dev_set_pmode"],nvm_dev_set_read_naddrs_max:[10,2,1,"c.nvm_dev_set_read_naddrs_max"],nvm_dev_set_write_naddrs_max:[10,2,1,"c.nvm_dev_set_write_naddrs_max"],nvm_geo:[11,4,1,"c.nvm_geo"],nvm_geo_pr:[11,2,1,"c.nvm_geo_pr"],nvm_meta_mode:[12,1,1,"c.nvm_meta_mode"],nvm_pmode:[14,1,1,"c.nvm_pmode"],nvm_pmode_str:[14,2,1,"c.nvm_pmode_str"],nvm_ret:[15,4,1,"c.nvm_ret"],nvm_ret_pr:[15,2,1,"c.nvm_ret_pr"],nvm_vblk:[16,4,1,"c.nvm_vblk"],nvm_vblk_alloc:[16,2,1,"c.nvm_vblk_alloc"],nvm_vblk_alloc_line:[16,2,1,"c.nvm_vblk_alloc_line"],nvm_vblk_erase:[16,2,1,"c.nvm_vblk_erase"],nvm_vblk_free:[16,2,1,"c.nvm_vblk_free"],nvm_vblk_get_addrs:[16,2,1,"c.nvm_vblk_get_addrs"],nvm_vblk_get_dev:[16,2,1,"c.nvm_vblk_get_dev"],nvm_vblk_get_naddrs:[16,2,1,"c.nvm_vblk_get_naddrs"],nvm_vblk_get_nbytes:[16,2,1,"c.nvm_vblk_get_nbytes"],nvm_vblk_get_pos_read:[16,2,1,"c.nvm_vblk_get_pos_read"],nvm_vblk_get_pos_write:[16,2,1,"c.nvm_vblk_get_pos_write"],nvm_vblk_pad:[16,2,1,"c.nvm_vblk_pad"],nvm_vblk_pr:[16,2,1,"c.nvm_vblk_pr"],nvm_vblk_pread:[16,2,1,"c.nvm_vblk_pread"],nvm_vblk_pwrite:[16,2,1,"c.nvm_vblk_pwrite"],nvm_vblk_read:[16,2,1,"c.nvm_vblk_read"],nvm_vblk_set_pos_read:[16,2,1,"c.nvm_vblk_set_pos_read"],nvm_vblk_set_pos_write:[16,2,1,"c.nvm_vblk_set_pos_write"],nvm_vblk_write:[16,2,1,"c.nvm_vblk_write"],nvm_ver_major:[17,2,1,"c.nvm_ver_major"],nvm_ver_minor:[17,2,1,"c.nvm_ver_minor"],nvm_ver_patch:[17,2,1,"c.nvm_ver_patch"],nvm_ver_pr:[17,2,1,"c.nvm_ver_pr"]}},objnames:{"0":["c","variable","variable"],"1":["c","enum","enum"],"2":["c","function","C function"],"3":["c","enumvalue","enumvalue"],"4":["c","struct","struct"]},objtypes:{"0":"c:variable","1":"c:enum","2":"c:function","3":"c:enumvalue","4":"c:struct"},terms:{"0x0":[1,5,6,12,14,22],"0x0000000000000002":[24,41],"0x000000000000000a":[24,38],"0x000000000001000a":38,"0x000000000002000a":38,"0x000000000003000a":38,"0x000000000004000a":38,"0x000000000005000a":38,"0x000000000006000a":38,"0x000000000007000a":38,"0x000000000008000a":38,"0x000000000064000a":19,"0x0000000000c8000a":38,"0x0000000001f4000a":38,"0x000000000440a642":19,"0x00000000084c8053":37,"0x000000010000000a":38,"0x000000010001000a":38,"0x000000010002000a":38,"0x000000010003000a":38,"0x000000010004000a":38,"0x000000010005000a":38,"0x000000010006000a":38,"0x000000010007000a":38,"0x000000010008000a":38,"0x000000010064000a":19,"0x0000000100c8000a":38,"0x0000000101f4000a":38,"0x000000020000000a":38,"0x000000020001000a":38,"0x000000020002000a":38,"0x000000020003000a":38,"0x000000020004000a":38,"0x000000020005000a":38,"0x000000020006000a":38,"0x000000020007000a":38,"0x000000020008000a":38,"0x000000020064000a":19,"0x0000000200c8000a":38,"0x0000000201f4000a":38,"0x000000030000000a":38,"0x000000030001000a":38,"0x000000030002000a":38,"0x000000030003000a":38,"0x000000030004000a":38,"0x000000030005000a":38,"0x000000030006000a":38,"0x000000030007000a":38,"0x000000030008000a":38,"0x000000030064000a":19,"0x0000000300c8000a":38,"0x0000000301f4000a":38,"0x000001000000000a":38,"0x000001000001000a":38,"0x000001000002000a":38,"0x000001000003000a":38,"0x000001000004000a":38,"0x000001000005000a":38,"0x000001000006000a":38,"0x000001000007000a":38,"0x000001000008000a":38,"0x000001000064000a":19,"0x0000010000c8000a":38,"0x0000010001f4000a":38,"0x000001010000000a":38,"0x000001010001000a":38,"0x000001010002000a":38,"0x000001010003000a":38,"0x000001010004000a":38,"0x000001010005000a":38,"0x000001010006000a":38,"0x000001010007000a":38,"0x000001010008000a":38,"0x000001010064000a":19,"0x0000010100c8000a":38,"0x0000010101f4000a":38,"0x000001020000000a":38,"0x000001020001000a":38,"0x000001020002000a":38,"0x000001020003000a":38,"0x000001020004000a":38,"0x000001020005000a":38,"0x000001020006000a":38,"0x000001020007000a":38,"0x000001020008000a":38,"0x000001020064000a":19,"0x0000010200c8000a":38,"0x0000010201f4000a":38,"0x000001030000000a":38,"0x000001030001000a":38,"0x000001030002000a":38,"0x000001030003000a":38,"0x000001030004000a":38,"0x000001030005000a":38,"0x000001030006000a":38,"0x000001030007000a":38,"0x000001030008000a":38,"0x000001030064000a":19,"0x0000010300c8000a":38,"0x0000010301f4000a":38,"0x000100000000000a":24,"0x000200000000000a":24,"0x000300000000000a":24,"0x0102000000000000":20,"0x0102000000000004":20,"0x02":[21,34,40],"0x0201000200c8000a":19,"0x0203000000000000":39,"0x020300000000002a":39,"0x020500000000002a":24,"0x0301000000000014":[24,41],"0x04010003000a00c8":37,"0x0500000000000190":[24,41],"0x0a0700000000014d":[24,41],"0x0d0700000000000a":41,"0x0e0700000000000a":41,"0x0f0700000000000a":41,"0x1":[1,5,6,7,12,14,22,38],"0x2":[1,5,22,38],"0x200":1,"0xaddr":[19,20,24,37],"0xff":38,"0xval":19,"10e2":0,"10e3":0,"10e5":0,"15th":35,"16kb":0,"32kb":0,"379ce192d401ab61":34,"4kb":0,"__cplusplu":1,"__liblightnvm":1,"__liblightnvm_h":1,"abstract":[1,16,29,36,41],"bj\u00f8rling":1,"byte":[1,4,8,11,16,38],"case":[0,38,41],"char":[1,8,10,14,19,34],"class":9,"const":[1,4,5,8,10,11,14,15,16,39],"default":[1,10,19,22,24,41],"enum":[1,5,6,7,12,14],"final":26,"function":[0,1,2,4,5,9,38],"gonz\u00e1l":1,"int":[1,4,5,7,8,10,11,14,16,17,34],"new":[0,1,16],"null":[1,5,8,16],"return":[1,4,5,8,10,14,16,17,34,39],"true":41,"try":34,"var":[19,38],"void":[1,4,5,7,8,10,11,15,16,17],"while":0,abcdefghijklmnop:38,abid:38,abl:[1,5],about:[0,1,4,10,16,17,29,34],abov:[0,1,26,34,37,38,40],academ:31,access:[0,1,4,5,14,29],accessor:[1,4],accord:[1,4],accordingli:[1,4],accross:38,achiev:[24,41],across:[0,24,38,41],actual:[0,1,38],addit:[0,38,41],addition:18,addr:[1,4,5,16,20,24,38,39,41],address:1,adher:38,adress:38,adv:34,advis:1,after:[0,39],against:[1,4],ahead:38,aio:31,aka:[1,5],align:[1,4,8,16,38],all:[0,1,5,10,20,24,26,29,37,38,41],alloc:[1,5],allow:[0,29,38],alpha:[1,10],alreadi:0,also:[29,38,41],although:[1,4],alwai:0,amount:[1,8,38,41],ani:[0,1,6,37],anoth:37,appli:[0,41],applic:[29,41],appropriatli:[1,8],approxim:41,apt:[26,34],arbitrari:[1,24,38],area:[0,1,2,10,19,22],argc:34,argument:[18,38],argv:34,aris:1,around:29,arrai:[1,4,5],assign:37,associ:[1,5,10,11,16,35],assum:[1,16,34,36],attribut:[1,40],author:1,avail:[0,2,18,26,29,31,32,34,36,38,41],back:[1,6],backend:[1,6,10,22,26],bad:1,basic:[29,34,38],bbt:[1,5,20,39],bbts_cach:[1,10,21,34,40],be_id:[21,34,40],becaus:38,been:[0,38],befor:[0,1,16],begin:[1,16,40],behav:41,behavior:[18,19,20,21,22,23,24],belong:0,below:40,between:[0,41],bin:[19,38],binari:1,bintrai:34,bit:[0,1,15],bjorl:35,blk:[1,4,5,16,19,20,21,24,34,37,38,39,40,41],blk_len:[21,34,40],blk_off:[21,34,40],block:[0,1,4],board:31,bonnet:35,bound:[0,1,4,7,10,19,22,38],boundari:[1,4,7],branch:32,breviti:[20,41],buf:[1,4,8,16],buffer:[1,4],build:[9,34],busi:1,bypass:26,cach:[0,1,10],calcul:0,call:1,caller:1,can:[0,1,4,19,24,29,32,37,38,39,41],cannot:9,capabl:[0,1],caus:[0,1],cdefghijklmnopqr:[19,38],ceas:0,cell:0,ch_bgn:[1,16,24],ch_end:[1,16,24],ch_len:[21,34,40],ch_off:[21,34,40],challeng:38,chang:[1,10,16,39],channel:[1,4,11,16,18,19,20,21,24],characterist:0,check:[1,4],chip:0,chmod:34,choos:38,chown:26,chunk:[1,4,11],clara:35,cli:[18,19,20,21,22,24,26,29,33],client:0,clock:41,clone:[26,31,34],close:[0,38],cmd:[1,6],cnex:31,cnexlab:[1,31],cnk:[1,11],code:[1,4,5,15,18],cognit:41,com:[1,26,31,32,34],come:41,command:[0,1],common:[0,29,37],commun:38,complet:[1,15],comput:1,concern:[29,37,38,41],condit:[1,15,18],confer:35,configur:[26,31],consequenti:1,consid:0,consist:[19,24,41],construct:[19,20,24,29,36,37,38,41],consult:[0,1,16],consum:41,contain:[0,1,2,4,27,38],content:[1,4,8,16,29],context:1,contract:1,contrast:[1,4,5],contribut:27,contributor:1,control:[0,1,22,31,41],conveni:[38,41],convers:37,convert:[1,4,19,37],copi:[1,5,39],copyright:1,core:29,correspond:36,count:[1,16,23],creat:[1,10,29,38],creation:41,current:0,cursor:[1,16],cycl:0,damag:1,data:[0,1,2,10,16,19,22,37,38],date:26,deadbeef:31,deal:0,dealloc:[1,5],deb:34,debug:18,decompos:0,decreas:0,deeper:34,defect:0,defin:[0,1,16],demonstr:41,depend:[0,26],describ:[0,1,5,18,30,38],descript:[1,7,31],descriptor:1,destin:[1,8],destroi:[1,5,10,16],detail:[29,34],dev2gen:19,dev:[1,4,5,10,11,16,19,20,21,24,26,34,36,37,38,39,40,41],dev_geo:[21,34,40],dev_path:[1,10,19,20,21,23,24],dev_ppaf:[21,34,40],dev_ppaf_mask:[21,34,40],die:0,differ:[0,1,10,16,38,41],direct:1,directli:0,directori:9,disabl:[1,10,19,22,38],disclaim:1,discuss:0,dist:26,distinct:41,distribut:1,disturb:0,dive:[34,38],doc:9,document:[1,2,41],doe:34,done:[0,37,38,39],down:37,downsid:0,doxi:9,doxygen:9,doxygenfunct:9,doxygenstruct:9,dragon:31,drive:31,drive_bbt:31,drive_metadata:31,drive_metas:31,drive_nam:31,dst:1,dual:[0,1,14,19,21,22,24,34,38,40,41],due:0,dump:[1,8,19,20,21,22,24,38],dure:0,each:[0,1,5,38,39,41],easi:29,ecc:0,echo:34,efghijklmnopqrst:19,eight:[0,19,38,41],either:[24,41],elaps:[24,38,41],emc:31,enabl:[1,10,19,31],encapsul:[1,4,15,24,29,41],end:[1,6,16,26],endif:1,enough:0,ensur:38,entir:[20,39],entiti:[1,16],entri:[20,39],enumer:[1,6,7,12],environ:[18,19,20,21],equal:[1,4],equival:[1,34,38,41],eras:[0,1,4,10,16,19,22],erase_naddrs_max:[21,34,40],errno:[1,4,5,8,10,16],error:[0,1,4,5,8,10,14,15,16,38],essenti:40,etc:34,even:1,event:1,everi:37,exampl:[0,18,19,20,21,24,34,38,41],exce:[1,4],exceed:[1,4],except:[1,37],execut:[0,1,38,39],exemplari:1,exist:39,expect:[1,19],experi:[29,34,41],experiment:18,explor:29,expos:[29,39,41],express:1,extens:33,extern:1,extra:0,facilit:[1,16],fail:[0,1,38],far:38,fast17:35,fast:35,fewer:38,fifth:37,figur:38,file:[1,8,19,24,35,38],fill:[1,4,5,8,16],filter:39,find:[1,9],fire:31,first:[24,38],fit:1,fix:[0,41],flag:[1,4,5,10,31],flash:[0,29,38],flat:41,flush:[1,5],focu:29,follow:[1,2,18,22,26,30,31,34,38,39],fopenmp:34,form:[1,2,16,19],format:[1,4],four:[0,19,24,38],free:[1,5,8,20,39],frequent:0,from:[1,5,8,9,16,18,19,21,24,26,29,34,37,38,40],from_geo:[19,24,37,38],from_hex:19,full:[0,19,20,21,23,24,38,41],fulli:[0,38],fundament:0,further:0,gbad:[20,39],gcc:34,gen2dev:[19,37],gen2lba:19,gen2off:19,gen:[19,37],gener:[0,1,4],geo:[1,4,8,11,21],geometr:[1,4],geometri:[1,4,8,10],get:[1,8,20,26,34,39],getter:40,ghijklmnopqrstuv:38,git:[26,31,32,34],github:[26,31,32,34],given:[1,4,5,7,8,10,11,14,15,16,18,19,24,37,38,39,40,41],goe:38,gonzalez:35,good:[0,1,5],granular:[0,1,4,8,37,38],great:41,greater:[1,8],grep:39,group:[1,2,4,11],grown:[1,5,20,39],grown_bad:[1,5],hand:38,handl:[0,1,4,5,10,16,37,38,40],hardwar:0,have:[0,1,16,34,38],hello:29,helper:38,here:[0,29,30,32],hexadecim:[19,37],hexdump:[19,38],hexidecim:[1,4],hierarchi:0,hint:[0,19,22,24,41],hkp:34,home:9,host:[1,5,31,39],how:[20,24,30,36],howev:[0,1,19,29,37,38,41],http:[19,20,21,24,26,31,34,35],human:[1,11],humanli:[1,4,5,7,15,16],identifi:[1,10],ifdef:1,ifndef:1,ijklmnopqrstuvwx:[19,38],implement:26,impli:1,improv:38,incident:1,includ:[1,2,29,34],inclus:[1,16],increas:[0,24,38,41],index:[1,16,29,37,41],indic:[1,5,8,10,16,26],indirect:1,info:[21,26,34,40],inform:[0,1,8,10,16,17],init:26,initi:[1,16],initialit:18,inner:27,input:[19,24,38],inquir:31,inspect:[2,19,38],instal:[26,31,34,36],instead:[1,38,41],integr:[29,41],intel:26,intend:29,interact:29,interest:[27,39],interfac:[0,1,10,16],intern:[0,1,16],interpret:[1,4,5],interrupt:1,introduc:[29,36,38,41],introduct:[29,34],inttyp:1,invalid:38,invok:[1,10,18,40],involv:40,ioctl:[1,6],issu:[0,36,38],itself:18,javier:[1,35],jump:29,kei:34,kernel:[26,32],keyserv:34,know:[1,4,38,40],known:41,kvm:31,lab:31,last:26,lastli:40,later:38,lba2gen:19,lba:[1,4,6],lba_index:31,lbbtabl:31,leak:0,length:[1,4,5],less:39,let:29,level:[0,1,4,5,15,37,38,41],liabil:1,liabl:1,lib:34,libc:[1,16,29,41],liblightnvm:[0,1,6,9,19,20,21,24],liblightnvm_spec:1,liblnvm:32,librari:[1,17],lightnvm:[19,20,21,24,31,35],like:[1,16,29],likewis:24,limit:[0,1,38],line_eras:[24,41],line_pad:24,line_read:[24,41],line_writ:[24,41],linear:41,linux:[31,32,34,35],list:[1,4,19,20,21,23,24,31,34],ll2pmode:31,llightnvm:34,lmetadata:31,lmetas:31,lnvm:[18,33],load:41,locat:[19,37],log:1,logic:[0,1,4,23],longer:26,look:[29,34],loss:1,lost:0,low:37,lower:[0,1,4,5,15],lpo:1,lun:[1,4,5,11,16,19,20,21,24,34,37,38,39,40,41],lun_bgn:[1,16,24],lun_end:[1,16,24],lun_len:[21,34,40],lun_off:[21,34,40],lund:1,mai:[0,31,33],main:34,major:[1,17,19,20,21,23,24],make:[18,26,31,34,39],manag:[1,5],mang:36,manifold:18,manner:[0,41],manufactur:[0,1,5],map:29,mark:[1,5,20,39],mark_b:20,mark_d:20,mark_f:20,mark_g:[20,39],mark_h:20,mask:[1,4,7],match:[1,8],materi:1,matia:[1,31,35],max:38,maximum:[1,10],mdt:31,mean:29,meant:18,meet:[1,16,30],member:[4,5,11,15,37],memori:[0,1,4,5,8,29,31],mention:38,merchant:1,mere:41,messag:26,met:[1,34],meta:[1,4,10,12,19,22],meta_mod:[1,10,21,34,40],meta_nbyt:[1,4,11,21,34,40],metadata:[1,4],might:[0,1,38],min:[1,4,16],minim:[0,1,4,8],minimum:[1,24],minor:[1,17,19,20,21,23,24],misc:1,miscellan:2,miss:26,mix:[1,16],mlc:0,mnopqrstuvwxyzab:[19,38],modif:1,modifi:[18,19,20,21,23,24,39],modul:29,more:37,most:[18,34,37],multi:0,multipl:[0,1,8,16,38,41],must:[0,1,4,8,16,38],naddr:[1,4,5,10,16,24,38,41],name:[1,21,34,40],namespac:[1,10,31],nand:[0,29,34],nbad:[1,5,20,39],nblk:[1,5,20,39],nblock:[1,11,21,34,40],nbyte:[1,4,8,11,24,38,41],nbytes_meta:1,nbytes_oob:[1,11],nchannel:[1,11,21,34,40],nchunk:[1,11],nda:0,ndmrk:[1,5,20,39],nearbi:0,necessari:0,need:[1,4,30,34,37,38],neglig:1,neighbor:0,next:0,ngbad:[1,5],nhmrk:[1,5,20,39],nine:38,nlbaf:31,nlun:[1,11,21,34,40],nmbyte:[24,38,41],none:[1,10],notat:26,note:[0,1,4,5,8,10,16,36,38],notic:1,notion:37,now:[1,16,38],npage:[1,11,21,34,40],npl_blk:[20,39],nplane:[1,11,21,34,38,40],nproc:26,npugrp:[1,11],npunit:[1,11],nsector:[1,11,21,34,38,40],nsectr:[1,11],number:[0,1,10,16,22,31,38],nvm:[1,4,10,16,19,20,21,23,24],nvm_:18,nvm_addr:1,nvm_addr_:19,nvm_addr_check:1,nvm_addr_dev2gen:1,nvm_addr_dev2lba:1,nvm_addr_dev2off:1,nvm_addr_eras:1,nvm_addr_gen2dev:1,nvm_addr_gen2lba:1,nvm_addr_gen2off:1,nvm_addr_lba2gen:1,nvm_addr_mark:[1,4],nvm_addr_off2gen:1,nvm_addr_pr:1,nvm_addr_prn:1,nvm_addr_read:1,nvm_addr_to_lpo:1,nvm_addr_writ:1,nvm_bbt:1,nvm_bbt_:20,nvm_bbt_alloc_cp:1,nvm_bbt_bad:[1,5],nvm_bbt_dmrk:[1,5],nvm_bbt_flush:1,nvm_bbt_flush_al:1,nvm_bbt_free:1,nvm_bbt_gbad:[1,5],nvm_bbt_get:1,nvm_bbt_hmrk:[1,5],nvm_bbt_mark:1,nvm_bbt_pr:1,nvm_bbt_set:1,nvm_bbt_state:1,nvm_bbt_state_pr:1,nvm_be:3,nvm_be_al:1,nvm_be_ani:[1,6,22],nvm_be_id:[1,3],nvm_be_ioctl:[1,6,22],nvm_be_ioctl_sysf:22,nvm_be_lba:[1,6],nvm_be_spdk:[1,6],nvm_be_sysf:[1,6],nvm_bound:[1,3],nvm_bounds_block:[1,7],nvm_bounds_channel:[1,7],nvm_bounds_chunk:[1,7],nvm_bounds_lun:[1,7],nvm_bounds_pag:[1,7],nvm_bounds_plan:[1,7],nvm_bounds_pr:[1,3],nvm_bounds_pugrp:[1,7],nvm_bounds_punit:[1,7],nvm_bounds_sector:[1,7],nvm_bounds_sectr:[1,7],nvm_buf_alloc:1,nvm_buf_alloca:1,nvm_buf_diff:1,nvm_buf_diff_pr:1,nvm_buf_fil:1,nvm_buf_fre:[1,8],nvm_buf_from_fil:1,nvm_buf_pr:1,nvm_buf_set:1,nvm_buf_set_alloc:1,nvm_buf_set_fil:1,nvm_buf_set_fre:1,nvm_buf_to_fil:1,nvm_cli_be_id:22,nvm_cli_erase_naddrs_max:22,nvm_cli_meta_pr:22,nvm_cli_noverifi:22,nvm_cli_pmod:[19,22,38],nvm_cli_read_naddrs_max:22,nvm_cli_write_naddrs_max:22,nvm_cmd_copi:1,nvm_cmd_eras:1,nvm_cmd_gbbt:1,nvm_cmd_gbbt_arb:1,nvm_cmd_idfi:1,nvm_cmd_read:1,nvm_cmd_rprt:1,nvm_cmd_rprt_arb:1,nvm_cmd_sbbt:1,nvm_cmd_write:1,nvm_dev:[1,4,5],nvm_dev_:21,nvm_dev_attr_pr:1,nvm_dev_clos:1,nvm_dev_get_:40,nvm_dev_get_bbts_cach:1,nvm_dev_get_be_id:1,nvm_dev_get_erase_naddrs_max:1,nvm_dev_get_fd:1,nvm_dev_get_geo:1,nvm_dev_get_lbaf:1,nvm_dev_get_mccap:1,nvm_dev_get_meta_mod:1,nvm_dev_get_nam:1,nvm_dev_get_nsid:1,nvm_dev_get_path:1,nvm_dev_get_pmod:1,nvm_dev_get_ppaf:1,nvm_dev_get_ppaf_mask:1,nvm_dev_get_quirk:1,nvm_dev_get_read_naddrs_max:1,nvm_dev_get_verid:1,nvm_dev_get_write_naddrs_max:1,nvm_dev_get_ws_min:1,nvm_dev_get_ws_opt:1,nvm_dev_name_len:1,nvm_dev_open:[1,4,5],nvm_dev_openf:1,nvm_dev_path_len:1,nvm_dev_pr:1,nvm_dev_set_bbts_cach:1,nvm_dev_set_erase_naddrs_max:1,nvm_dev_set_meta_mod:1,nvm_dev_set_pmod:1,nvm_dev_set_quirk:1,nvm_dev_set_read_naddrs_max:1,nvm_dev_set_write_naddrs_max:1,nvm_dump:19,nvm_flag_default:1,nvm_flag_pmode_du:[1,14],nvm_flag_pmode_quad:[1,14],nvm_flag_pmode_sngl:[1,14],nvm_flag_scrbl:1,nvm_geo:[1,4,8,10],nvm_geo_pr:1,nvm_lba_:23,nvm_lba_pread:1,nvm_lba_pwrit:1,nvm_meta:3,nvm_meta_mod:[1,3],nvm_meta_mode_:[1,10],nvm_meta_mode_alpha:[1,12],nvm_meta_mode_const:[1,12],nvm_meta_mode_non:[1,12],nvm_mode:3,nvm_naddr_max:1,nvm_pmode:[1,3],nvm_pmode_str:[1,3],nvm_quirk:1,nvm_quirk_nsid_by_nameconv:1,nvm_quirk_oob_2lrg:1,nvm_quirk_oob_read_1st4bytes_nul:1,nvm_quirk_pmode_erase_runrol:1,nvm_quirk_semi20:1,nvm_ret:[1,3,4,5],nvm_ret_pr:[1,3],nvm_spec_bbt:1,nvm_spec_chunk_st:1,nvm_spec_idfi:1,nvm_spec_lbaf:1,nvm_spec_ppaf_nand:1,nvm_spec_ppaf_nand_mask:1,nvm_spec_rprt:1,nvm_vblk:[1,10],nvm_vblk_:24,nvm_vblk_alloc:1,nvm_vblk_alloc_lin:1,nvm_vblk_eras:1,nvm_vblk_free:1,nvm_vblk_get_addr:1,nvm_vblk_get_dev:1,nvm_vblk_get_naddr:1,nvm_vblk_get_nbyt:1,nvm_vblk_get_pos_read:1,nvm_vblk_get_pos_writ:1,nvm_vblk_pad:1,nvm_vblk_pr:1,nvm_vblk_pread:1,nvm_vblk_pwrite:1,nvm_vblk_read:1,nvm_vblk_set_pos_read:1,nvm_vblk_set_pos_writ:1,nvm_vblk_write:1,nvm_ver:3,nvm_ver_major:[1,3],nvm_ver_minor:[1,3],nvm_ver_patch:[1,3],nvm_ver_pr:[1,3],nvme0n1:[1,10,19,20,21,24,34,36,37,38,39,40,41],nvme:[1,10,15,18,31,33,37],obtain:[1,4,5,10,14,16,29,38],occur:[0,38],off2gen:19,off:[1,4],off_t:1,offer:[29,41],offset:[1,4,16,23],often:[0,38],omit:[20,41],onli:[0,38],oob:[1,11],opaqu:[1,10,16],open:[0,1,10,18,21],openchannelssd:[9,31,32,34],oper:[0,24,29,30],opqrstuvwxyzabcd:19,opt:[1,26],optim:[0,1,29],option:[1,19,20,21,24,30,38],order:[1,16,31],org:35,organ:0,other:[0,1],otherwis:1,our:38,out:[0,1,10,19,22,34,39],output:[9,19,20,24,26,34,37,38,39,40,41],over:[0,41],pack:[1,4],packag:[0,26,34],pad:[1,16,24],page:[0,1,4,11,19,29,37,38],page_nbyt:[1,11,21,34,40],parallel:[0,1,4,11,24,29,38,41],param:1,paramet:[4,5,8,10,14,15,16,19,20,24,33,37],part:[36,37,40],particular:1,pass:26,patch:[1,17,19,20,21,23,24],path:[1,8,10,19,21,24,34,36,40],payload:[19,24,38],pci:26,peak:38,peal:29,per:[0,1,11],perform:[0,1,16,19,29,38],permit:1,perror:34,persist:[0,1,5,39],pg_len:[21,34,40],pg_off:[21,34,40],philipp:35,physic:[0,1,4,16,19,24,29],pitfal:29,pkgdep:26,pl_blk:[20,39],pl_len:[21,34,40],pl_off:[21,34,40],place:[26,29,34,38],plane:[0,1,4,10,11,14,16,19,22],plane_mod:[1,10],pleas:31,pmode:[1,10,14,19,21,24,34,38,40,41],point:38,pointer:[1,4,5,8,15,16],posit:[1,16],possibl:1,power:41,ppa:[1,4],practic:34,pread:[1,16,23,29],prefix:18,prerequisit:29,present:35,prevent:0,primari:38,primarili:27,primit:41,print:[1,4,5,7,8,10,11,15,16,17,19,20,21,24,34],privilig:[20,39],process:0,procur:1,produc:40,profit:1,program:0,project:9,prone:0,properli:40,proportion:41,protect:0,provid:[0,1,2,5,18,19,24,26,29,36,38,41],pseudo:[1,10,12],pug:[1,4,11],pugrp:[1,4],punit:[1,4],pure:41,purpos:[1,4,18,31],put:34,pwrite:[1,16,23],qemu:31,qlc:0,qrstuvwxyzabcdef:38,quad:[1,14,22],quick:29,quirk:1,radian:31,raid:0,rang:24,rate:0,raw:[1,2,4],read_meta:1,read_naddrs_max:[21,34,40],read_wm:19,readabl:[1,4,5,7,11,15,16],readili:34,receiv:38,recent:26,recv:34,redistribut:1,reduc:41,refer:[0,29],regard:41,regular:1,rel:[19,37],relax:38,remain:[1,16,38],repeat:19,replic:0,repo:32,repons:1,report:1,repres:37,represent:[1,4,5,11,14,15,19],reproduc:1,request:31,requir:[0,20,39],research:31,reserv:[1,5,39],reset:[1,16],resid:[1,5],respect:[0,1,4],respons:[1,41],result:[0,1,4,5,15,38],ret:[1,4,5,15],retain:1,retent:0,retir:0,retriev:[1,5,10,16],rewritten:0,right:[1,29],rling:35,rms325:31,round:38,rule:0,run:[26,31],same:[0,1,16,38,41],santa:35,satisfi:38,scale:41,scatter:38,scrambler:1,script:26,sdk:31,search:29,sec:[1,4,19,20,21,24,34,37,38,39,40,41],sec_len:[21,34,40],sec_off:[21,34,40],second:38,section:[0,2,18,19,20,21,23,24,27,30,34,38,41],sector:[0,1,4,5,11,19,37,38],sector_nbyt:[1,4,11,21,34,40],sectr:[1,4],see:[1,5,10,11,16,18,19,20,21,23,24,33,34,41],seen:38,send:[1,10,38],sensit:1,sequenc:[0,19],sequenti:0,serial:31,serv:18,servic:1,session:35,set:[1,4,5,8,10,16,18,19,20,22],set_b:20,set_d:20,set_eras:[24,41],set_f:20,set_g:20,set_h:20,set_pad:24,set_read:[24,41],set_writ:[24,41],setup:[26,31,34],sever:0,shall:1,share:0,should:[0,1,10,26,34],show:37,shown:40,shrink:0,side:[1,5],similar:[0,26,34],simon:1,sinc:[0,26],singl:[0,1,14,38],size:[0,1,4,8,16,38],size_t:[1,8,11,16],slc:0,slightli:0,slund:1,smallest:38,sneak:38,sngl:[19,38],softmmu:31,softwar:[1,41],some:0,sourc:[1,8,18,26,32,34],space:1,span:[1,16],spdk:[1,6],spec:[1,4,11],special:1,specif:[0,1,4,10,29],specifi:[37,38,41],speedup:41,src:1,ssd:[1,18,21],ssize_t:[1,4,16],ssw:[21,34,40],start:[1,16,29,30],state:[1,5,19],statu:[1,4,5,15,38],stdint:1,stdio:34,stdlib:1,stdout:[1,8,19,20,21,22,24],storag:[0,35],store:[0,1,4,5,38],strict:1,string:[1,14],struct:[1,4,5,8,10,11,15,16,26,34,37,39],structur:[1,2,4,5,37,40],stuvwxyzabcdefgh:[19,38],submodul:26,subsect:2,subset:[29,41],substitut:1,subsystem:35,succe:[0,38],success:[1,4,5,8,10,14,16,26,38],successfulli:[0,38],sudo:[20,26,31,34,39],suffic:34,suit:34,supplement:18,support:[29,30,31],sure:26,synthet:[1,16,19,24],sysadmin:[20,39],sysf:[1,6],system:[1,16,24,26,29,30,31],tabl:1,take:[1,29,37,38],target:31,task:38,tbyte:[1,11,21,34,40],technic:35,techniqu:0,technolog:35,tediou:38,tee:34,ten:24,termin:40,test:[18,26],textual:19,than:[1,8,38],thei:38,them:1,theori:1,therebi:29,therefor:[38,41],thereof:29,thi:[0,1,4,5,18,26,27,34,36,38,41],thin:29,those:27,three:0,threshold:0,through:[33,36,39],throughout:36,thu:[38,39,41],thumb:0,time:[0,38,41],tlc:0,tmbyte:[21,34,40],tmp:[19,38],tool:[18,29,30],tort:1,total:[1,5,11,38],traddr:26,translat:37,transport:22,travi:9,trip:38,tutori:[29,34],two:38,type:[0,1],typic:0,ubuntu:[26,34],uint16_t:[1,4,5],uint32_t:[1,5,15],uint64_t:[1,4,5,15],uint8_t:[1,5],under:0,underli:39,understand:0,unexpect:38,union:[1,4,11],unit:[0,1,4,11,18,24,26,29,38,41],unittest:26,unkn:[1,14],unknown:38,unless:0,unrecover:0,unsign:[1,4],updat:[1,5,16],upgrad:26,upon:31,upper:[0,38],url:35,usag:[18,19,20,21,23,24],usenix:35,user:[0,1,4,26],usual:0,util:[24,29,41],uuid:26,uvwxyzabcdefghij:19,val:[1,4,19],valid:[1,5],valu:[1,4,5,6,7,12,14,22,37,40],variabl:[18,19,20,21],variou:0,vblk:[1,16,24,38,41],vector:[19,29,36],vendor:0,ver:[19,20,21,23,24],verid:[1,10,11,21,34,40],verif:22,version:[1,17],via:[29,38],virtf:31,virtual:1,wai:[0,1],wall:41,want:[1,16],warranti:1,weak:41,wear:0,well:[1,5],were:0,what:[29,30,34,38],when:[0,1,10,22,24,29,38,40,41],where:[1,8,38],whether:[1,4,10],which:[0,1,4,5,19,22,34,38,39,40,41],within:[0,1,4,16,37,38],without:[1,18,19,38],work:[0,27,29,34,38,40,41],workload:41,world:34,worri:[1,4],worth:38,would:26,wrap:34,wrapper:29,write_meta:1,write_naddrs_max:[21,34,40],write_wm:19,written:[0,1,16,38,41],wrong:38,www:[26,35],wxyzabcdefghijkl:38,x86_64:31,xenial:34,xml:9,yield:[26,37,38,39,40,41],you:[1,16,29,30,32,33,34],your:[31,34],yzabcdefghijklmn:19,zero:[1,8,37]},titles:["Background","Public Header","C API","Miscellaneous","nvm_addr - Addressing","nvm_bbt - Bad-Block-Table","nvm_be","nvm_bounds","nvm_buf - Buffer Allocation","nvm_cmd - Raw Commands","nvm_dev - Device Management","nvm_geo - Geometry","nvm_meta","nvm_mode","nvm_pmode","nvm_ret","nvm_vblk - Virtual Block","nvm_ver","Command-Line Interface","nvm_addr","nvm_bbt","nvm_dev","Environment Variables","nvm_lba","nvm_vblk","&lt;no title&gt;","SPDK","Developer Information","&lt;no title&gt;","User space I/O library for Open-Channel SSDs","Prerequisites","Open-Channel SSD","Operating System Support","Management Tool","Quick Start","References","Tutorial Introduction","Physical Addressing","Vectorized IO to NAND media","Bad-Block-Table","Obtaining device information","Virtual Block"],titleterms:{"public":1,address:[4,19,37],alloc:8,api:[2,34],architectur:0,background:0,bad:[5,39],befor:38,block:[5,16,20,24,39,41],buffer:8,channel:[29,31,34],cli:34,command:[9,18],compil:[26,34],constraint:0,contigu:38,develop:27,devic:[0,10,19,21,26,37,40],environ:22,eras:[24,38],failur:0,format:[19,37],gener:[19,37],geometri:11,header:1,hello:34,identifi:26,indic:29,inform:[21,27,40],interfac:18,introduct:36,liblightnvm:26,librari:29,line:[18,24,41],manag:[10,33],maximum:38,media:[0,38],minim:38,minimum:38,miscellan:3,mode:0,nand:38,non:38,nvm_addr:[4,19],nvm_addr_check:4,nvm_addr_dev2gen:4,nvm_addr_eras:4,nvm_addr_gen2dev:4,nvm_addr_gen2lba:4,nvm_addr_gen2off:4,nvm_addr_lba2gen:4,nvm_addr_off2gen:4,nvm_addr_pr:4,nvm_addr_prn:4,nvm_addr_read:4,nvm_addr_writ:4,nvm_bbt:[5,20],nvm_bbt_alloc_cp:5,nvm_bbt_flush:5,nvm_bbt_flush_al:5,nvm_bbt_free:5,nvm_bbt_get:5,nvm_bbt_mark:5,nvm_bbt_pr:5,nvm_bbt_set:5,nvm_bbt_state:5,nvm_bbt_state_pr:5,nvm_be:6,nvm_be_id:6,nvm_bound:7,nvm_bounds_pr:7,nvm_buf:8,nvm_buf_alloc:8,nvm_buf_fil:8,nvm_buf_from_fil:8,nvm_buf_pr:8,nvm_buf_to_fil:8,nvm_cmd:9,nvm_cmd_admin:9,nvm_cmd_pr:9,nvm_cmd_user:9,nvm_cmd_vadmin:9,nvm_cmd_vuser:9,nvm_cmd_vuser_pr:9,nvm_dev:[10,21],nvm_dev_clos:10,nvm_dev_get_bbts_cach:10,nvm_dev_get_be_id:10,nvm_dev_get_erase_naddrs_max:10,nvm_dev_get_geo:10,nvm_dev_get_meta_mod:10,nvm_dev_get_nsid:10,nvm_dev_get_pmod:10,nvm_dev_get_read_naddrs_max:10,nvm_dev_get_verid:10,nvm_dev_get_write_naddrs_max:10,nvm_dev_open:10,nvm_dev_openf:10,nvm_dev_pr:10,nvm_dev_set_bbts_cach:10,nvm_dev_set_erase_naddrs_max:10,nvm_dev_set_meta_mod:10,nvm_dev_set_pmod:10,nvm_dev_set_read_naddrs_max:10,nvm_dev_set_write_naddrs_max:10,nvm_geo:11,nvm_geo_pr:11,nvm_lba:23,nvm_meta:12,nvm_meta_mod:12,nvm_mode:13,nvm_pmode:14,nvm_pmode_str:14,nvm_ret:15,nvm_ret_pr:15,nvm_vblk:[16,24],nvm_vblk_alloc:16,nvm_vblk_alloc_lin:16,nvm_vblk_eras:16,nvm_vblk_free:16,nvm_vblk_get_addr:16,nvm_vblk_get_dev:16,nvm_vblk_get_naddr:16,nvm_vblk_get_nbyt:16,nvm_vblk_get_pos_read:16,nvm_vblk_get_pos_writ:16,nvm_vblk_pad:16,nvm_vblk_pr:16,nvm_vblk_pread:16,nvm_vblk_pwrite:16,nvm_vblk_read:16,nvm_vblk_set_pos_read:16,nvm_vblk_set_pos_writ:16,nvm_vblk_write:16,nvm_ver:17,nvm_ver_major:17,nvm_ver_minor:17,nvm_ver_patch:17,nvm_ver_pr:17,obtain:40,open:[29,31,34],oper:32,physic:[31,37],plane:[24,41],prerequisit:30,quick:34,raw:9,read:[0,24,38],refer:35,retriev:[20,21],run:34,scope:37,set:[24,41],space:29,span:[24,41],spdk:26,ssd:[29,31,34],start:34,state:20,submit:19,support:32,system:32,tabl:[5,29,39],tool:33,tutori:36,unbind:26,updat:20,usag:26,user:29,variabl:22,vector:38,virtual:[16,31,41],write:[0,24,38]}})