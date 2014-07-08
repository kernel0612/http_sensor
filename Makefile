ACE_include=-I $(ACE_ROOT)
ACE_lib=-L $(ACE_ROOT)/lib/
make:
	g++ -g -o http_sensor main.cpp cap_http.cpp interaction.cpp Interaction_List.cpp my_ace_guard.cpp my_berkeleyDB.cpp output_to_db.cpp output_to_file.cpp proc_capCnt_block.cpp proc_output_interaction.cpp proc_reg_rules.cpp TimeOut_Handler.cpp PQ.cpp $(ACE_include) $(ACE_lib)  -lACE -ldb -lz -luuid -lnids -lpq
clean:
	rm http_sensor
