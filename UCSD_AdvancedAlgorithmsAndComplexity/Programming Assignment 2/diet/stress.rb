COEF_CONST = -50

def generate_input
	n = 2 + rand(3)
	m = 2 + rand(3)
	coeffs = []
	n.times do
		coeff_line = []
		m.times{ coeff_line.push(COEF_CONST + rand(100)) }
		coeffs.push coeff_line
	end

	answers = []
	n.times{answers.push(-100 + rand(200))}

	pleasures = []
	n.times{pleasures.push(-100 + rand(200))}

	coeffs_string = coeffs.map{|arr| arr.join(' ')}.join("\n")
	answers_string = answers.join(' ')
	pleasures_string = pleasures.join(' ')

	"#{n} #{m}\n#{coeffs_string}\n#{answers_string}\n#{pleasures_string}"
end

def apply_input_to_both_scripts
end

def compare_results
end

test_success = true
test_cnt = 100
cnt = 0
while cnt < test_cnt and test_success do
	cnt += 1
	puts cnt
	input = generate_input()
	File.write('input.txt', input)
	cmd_cpp = "./a.out < input.txt"
	res_cpp = %x[ #{cmd_cpp} ]

	cmd_py = "python3 verify.py < input.txt"
	res_py = %x[ #{cmd_py} ]

	if res_cpp != res_py
		label_cpp, answer_cpp = res_cpp.split("\n")
		label_py, answer_py = res_py.split("\n")


		if label_cpp != label_py
			test_success = false
			puts "Input:"
			puts input
			puts "C++ solution:"
			puts res_cpp
			puts "Python solution:"
			puts res_py
		end
	end
end
