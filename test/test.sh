#!/bin/bash

#-------------------------------------
# USER SETTING
#-------------------------------------
readonly MINISHELL_DIR="../"
readonly MINISHELL_EXE="minishell"
readonly MINISHELL_PROMPT="$ "
#-------------------------------------

readonly MINISHELL_PATH="${MINISHELL_DIR}${MINISHELL_EXE}"
readonly LOG_FILE_NAME="result.log"

source help/helper.sh

rm -f "${TMP_DIR}/*" "${LOG_FILE}"

build_minishell () {
	make -C "${MINISHELL_DIR}"
}

exec_minishell (){
	"${MINISHELL_PATH}"
}

clean_minishell () {
	make -C "${MINISHELL_DIR}" fclean
}

if [[ $1 == clean ]]; then
	clean_minishell
	exit
elif [[ -e ${MINISHELL_PATH} ]]; then
	echo "minishell exist!"
else
	build_minishell
fi
mkdir ${TMP_DIR}


run_all_tests () {
	for file in $(ls ${CASES_DIR} | sed 's/\.txt//'); do
		echo -e "\n\n---------------------------------"
		echo "${file}TEST"
		echo -e "---------------------------------"
		echo -e "\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" >> ${LOG_FILE}
		echo "            ${file}TEST" >> ${LOG_FILE}
		echo -e "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n" >> ${LOG_FILE}
		run_tests "${file}"
	done
	print_result
}

print_result () {
	echo -e "\n\n\nRESULT"
	if [[ ${result_ok} -eq ${result_all} ]]; then
		printf "\t${COLOR_GREEN}${result_ok} / ${result_all}${COLOR_RESET}\t\n"
	else
		printf "\t${COLOR_RED}${result_ok} / ${result_all}${COLOR_RESET}\t\n"
	fi
}

run_tests () {
	while read -r line; do
		test_cmd="${line}"
		run_shell "$test_cmd" "${CASES_DIR}/$1.txt"
		check_diff "$test_cmd"
		output_log "$test_cmd"
	done < "${CASES_DIR}/$1.txt"
}

run_minishell () {
	echo -e "$1" | "${MINISHELL_PATH}" > ${MINISHELL_STDOUT_FILE} 2> ${MINISHELL_STDERR_FILE}
	minishell_status=$?
	sed -i "" -e "/${MINISHELL_PROMPT}/d" ${MINISHELL_STDOUT_FILE}
}

run_bash () {
	echo -e "$1" | bash > ${BASH_STDOUT_FILE} 2> ${BASH_STDERR_FILE}
	bash_status=$?
}

run_shell () {
	run_minishell "$test_cmd"
	clean
	# if [ $2 = "${CASES_DIR}/cd.txt" ]; then
	# 	clean
	# fi
	run_bash "$test_cmd"
	clean
	# if [ $2 = "${CASES_DIR}/cd.txt" ]; then
	# 	clean
	# fi
}

check_diff () {
	diff_stdout=$(diff ${BASH_STDOUT_FILE} ${MINISHELL_STDOUT_FILE})
	diff_stderr=$(diff ${BASH_STDERR_FILE} ${MINISHELL_STDERR_FILE})
	if is_ok ; then
		print_ok
		let result_ok++
	else
		print_ko
		let result_ko++
	fi
	let result_all++
}

print_ok () {
	printf "${COLOR_GREEN}[✔ ]${COLOR_RESET}"
}

print_ko () {
	printf "${COLOR_RED}[✔ ]${COLOR_RESET}"
}

is_ok () {
	if [[ -z "${diff_stdout}" ]] && [[ ${bash_status} -eq ${minishell_status} ]]; then
		return 0
	fi
	return 1
}

print_case () {
	echo -n "case: $1"
}

output_log () {
	echo -e "\n-------------------------" >> ${LOG_FILE}
	if is_ok ; then
		echo -n "[OK ] " >> ${LOG_FILE}
	else
		echo -n "[KO ] " >> ${LOG_FILE}
	fi
	echo $(print_case "$1") >> ${LOG_FILE}
	echo "-------------------------" >> ${LOG_FILE}
	echo "# minishell: stdout" >> ${LOG_FILE}
	cat "${MINISHELL_STDOUT_FILE}" >> ${LOG_FILE}
	echo "# bash: stdout" >> ${LOG_FILE}
	cat "${BASH_STDOUT_FILE}" >> ${LOG_FILE}
	echo "# minishell: stderr" >> ${LOG_FILE}
	cat "${MINISHELL_STDERR_FILE}" >> ${LOG_FILE}
	echo "# bash: stderr" >> ${LOG_FILE}
	cat "${BASH_STDERR_FILE}" >> ${LOG_FILE}
	echo "# minishell: exit status = ${minishell_status}" >> ${LOG_FILE}
	echo "# bash     : exit status = ${bash_status}" >> ${LOG_FILE}
}

clean () {
	rm -rf ./dir
	rm -rf ./DIR
	rm -rf ./hello
	rm -rf ./a
	rm -rf ./b
	rm -rf ./link
	rm -f ./testfile
	rm -rf ./a:b
	rm -rf cmd
	rm -rf file
	rm -rf ./symdir
	rm -f *.txt
}

main () {
	run_all_tests
	rm -rf "${TMP_DIR}"
	clean
}

main $@
