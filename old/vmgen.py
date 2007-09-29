import copy 

class Var:
	def __init__(self, typ, name):
		self.typ = typ
		self.name = name
		
	def __repr__(self):
		return "<Var %s %s>" % (self.typ, self.name)
		
def parse_vars(s):
	s = s.split(',')
	v = []
	for var in s:
		var = var.strip()
		if var == '':
			continue
		parts = var.split(' ')
		name = parts[-1]
		typ = ' '.join(parts[0:-1])
		v.append(Var(typ,name))
	return v

class Instr:
	def __init__(self, **kwargs):
		self.name = '???'
		self.data = ''
		self.ins = ''
		self.outs = ''
		self.code = ''
		self.types = ''
		self.typ = ''
		self.incPC = True
		for key,val in kwargs.iteritems():
			setattr(self,key,val)
		#print parse_vars(self.ins)
		self.dataVars = parse_vars(self.data)
		self.inVars = parse_vars(self.ins)
		self.outVars = parse_vars(self.outs)
		
		parts = self.name.split(':')
		self.symbol = 'OP_' + parts[0].upper()
		if len(parts) > 1:
			self.symbol += '_' + parts[1].upper()
			
		self.struct = 'Instr_' + parts[0]
		if len(parts) > 1:
			self.struct += '_' + parts[1]
			
		self.mnemonic = parts[0]
		if len(parts) > 1:
			self.mnemonic += '_' + parts[1]
			
	def type_code(self,s):
		xlat = {'uint8':'b',
				'int8':'sb',
				'uint16':'us',
				'int16':'s',
				'uint32':'ui',
				'int32':'i',
				'uint64':'uq',
				'int64':'q',
				'float32':'f',
				'float64':'d'}
		if s in xlat:
			return xlat[s]
		else:
			return s
							
	def struct_code(self):
		s = ''
		s += 'struct %s\n' % self.struct
		s += '{\n'
		s += '\tuint32 opcode;\n'
		
		for v in self.dataVars:
			s += '\t%s %s;\n' % (v.typ, v.name)
		
		s += '};\n\n'
		s = self.replace_types(s)
		return s
		
	def expand(self):
		if self.types == '':
			return [self]
		else:
			types = self.types.split(',')
			results = []
			for t in types:
				x = copy.copy(self)
				x.typ = t
				typecode = self.type_code(t)
				x.symbol += '_' + typecode.upper()
				x.struct += '_' + typecode
				x.mnemonic += '_' + typecode
				results.append(x)
			return results
	
	def replace_types(self,s):
		return s.replace('$T', self.typ)
		
	def assemble_code(self):
		s = ''
		
		s += 'void %s(' % self.mnemonic
		
		parms = []
		for v in self.dataVars:
			parms.append('%s _%s' % (v.typ, v.name))
		s += ', '.join(parms)
		s += ')\n'
		s += '{\n'
		
		s += '\t%s instr;\n' % self.struct
		s += '\tinstr.opcode = %s;\n' % self.symbol
		
		for v in self.dataVars:
			s += '\tinstr.%s = _%s;\n' % (v.name, v.name)
			
		s += '\tAddInstr((Instr*)&instr, sizeof(instr));\n'
		
		s += '}\n\n'
		s = self.replace_types(s)
		return s
		
	def disassemble_code(self):
		s = ''
		
		s += 'case %s:\n' % self.symbol
		s += '{\n'
		
		s += '\tprintf("%s");\n' % self.mnemonic
		
		first = True
		for v in self.dataVars:
			if first:
				first = False
				s += '\tprintf(" ");\n'
			else:
				s += '\tprintf(", ");\n'
			s += '\tprintData( ((%s*)instr)->%s );\n' % (self.struct, v.name)
		
		s += '\tbreak;\n'
		s += '}\n\n'
		s = self.replace_types(s)
		return s
	
	def interpret_code(self):
		s = ''
		s += 'case %s:\n' % self.symbol
		s += '{\n'
		
		for v in self.dataVars:
			s += '\t%s& %s = ((%s*)instr)->%s;\n' % (v.typ, v.name, self.struct, v.name)
			
		for v in self.inVars:
			s += '\t%s %s;\n' % (v.typ, v.name)
			
		for v in self.outVars:
			s += '\t%s %s;\n' % (v.typ, v.name)
			
		for v in self.inVars:
			s += '\tPOP(%s, %s);\n' % (v.typ, v.name)
			
			
		s += '\t' + self.code
		s += '\n'
			
		for v in self.outVars:
			s += '\tPUSH(%s, %s);\n' % (v.typ, v.name)
			
		if self.incPC:
			s += '\tINC_PC;\n'
		
		s += '\tbreak;\n'
		s += '}\n\n'
		
		s = self.replace_types(s)
		
		return s
	
#-----------------------------------------------------------------------------

instrs = [

	Instr(name='jump',
			data='int32 target',
			incPC = False,
			code='SET_PC(target);'
		),
		
	Instr(name='jump_true',
			data='int32 target',
			ins='int32 cond',
			incPC = False,
			code="""
				if(cond != 0) {
					SET_PC(target);
				} else {
					INC_PC;
				}
				"""
		),
		
	Instr(name='jump_false',
			data='int32 target',
			ins='int32 cond',
			incPC = False,
			code="""
				if(cond == 0) {
					SET_PC(target);
				} else {
					INC_PC;
				}
				"""
		),
		
	# TODO: Ret
	
	Instr(name='ret',
			code='/*TODO*/\nreturn;'),
			
	Instr(name='nop',
			code=''),
		
	# ...
	
	Instr(name='add',
			types='int32,uint32,int64,uint64,float32,float64',
			ins='$T a, $T b',
			outs='$T out',
			code='out = a+b;'
		),
		
	Instr(name='sub',
			types='int32,uint32,int64,uint64,float32,float64',
			ins='$T a, $T b',
			outs='$T out',
			code='out = a-b;'
		),
		
	Instr(name='mul',
			types='int32,uint32,int64,uint64,float32,float64',
			ins='$T a, $T b',
			outs='$T out',
			code='out = a*b;'
		),
		
	Instr(name='div',
			types='int32,uint32,int64,uint64,float32,float64',
			ins='$T a, $T b',
			outs='$T out',
			code='out = a/b;'
		),
		
	Instr(name='mod',
			types='int32,uint32,int64,uint64',
			ins='$T a, $T b',
			outs='$T out',
			code='out = a%b;'
		),
		
	Instr(name='and',
			types='int32,uint32,int64,uint64',
			ins='$T a, $T b',
			outs='$T out',
			code='out = a & b;'
		),
		
	Instr(name='or',
			types='int32,uint32,int64,uint64',
			ins='$T a, $T b',
			outs='$T out',
			code='out = a | b;'
		),
		
	Instr(name='xor',
			types='int32,uint32,int64,uint64',
			ins='$T a, $T b',
			outs='$T out',
			code='out = a ^ b;'
		),
		
	Instr(name='compl',
			types='int32,uint32,int64,uint64',
			ins='$T a',
			outs='$T out',
			code='out = ~a;'
		),
		
	Instr(name='shl',
			types='int32,uint32,int64,uint64',
			ins='$T a, uint32 b',
			outs='$T out',
			code='out = a << b;'
		),
		
	Instr(name='shr',
			types='int32,uint32,int64,uint64',
			ins='$T a, uint32 b',
			outs='$T out',
			code='out = a >> b;'
		),
		
	Instr(name='neg',
			types='int32,int64,float32,float64',
			ins='$T a',
			outs='$T out',
			code='out = -a;'
		),
		
	Instr(name='lt',
			types='int32,uint32,int64,uint64,float32,float64',
			ins='$T a, $T b',
			outs='uint32 out',
			code='out = (a < b) ? 1 : 0;'
		),
		
	Instr(name='le',
			types='int32,uint32,int64,uint64,float32,float64',
			ins='$T a, $T b',
			outs='uint32 out',
			code='out = (a <= b) ? 1 : 0;'
		),
		
	Instr(name='gt',
			types='int32,uint32,int64,uint64,float32,float64',
			ins='$T a, $T b',
			outs='uint32 out',
			code='out = (a > b) ? 1 : 0;'
		),
		
	Instr(name='ge',
			types='int32,uint32,int64,uint64,float32,float64',
			ins='$T a, $T b',
			outs='uint32 out',
			code='out = (a >= b) ? 1 : 0;'
		),
		
	Instr(name='eq',
			types='int32,uint32,int64,uint64,float32,float64',
			ins='$T a, $T b',
			outs='uint32 out',
			code='out = (a == b) ? 1 : 0;'
		),
		
	Instr(name='ne',
			types='int32,uint32,int64,uint64,float32,float64',
			ins='$T a, $T b',
			outs='uint32 out',
			code='out = (a != b) ? 1 : 0;'
		),
		
	# ...
		
	Instr(name='load_const',
			types='int32,uint32,int64,uint64,float32,float64',
			data='$T value',
			outs='$T out',
			code='out = value;'
		),
		
	Instr(name='pop',
			types='int32,uint32,int64,uint64,float32,float64',
			ins='$T top',
			code='top;'
		),
		
	Instr(name='print:i',
			ins='int32 a',
			code='printf("%d\\n",a);'
		),
]

#-----------------------------------------------------------------------------


def gen_code():
	global instrs
	instrs2 = []
	for i in instrs:
		instrs2 += i.expand()
	instrs = instrs2

	s = ''
	
	s += '#ifdef SS_GEN_SYMBOLS\n'
	for i in instrs:
		s += '%s,\n' % i.symbol
	s += '#endif\n\n'
	
	s += '#ifdef SS_GEN_STRUCTS\n\n'
	for i in instrs:
		s += i.struct_code()
	s += '#endif\n\n'
	
	
	s += '#ifdef SS_GEN_ASSEMBLE\n\n'
	for i in instrs:
		s += i.assemble_code()
	s += '#endif\n\n'	
	
	
	s += '#ifdef SS_GEN_DISASSEMBLE\n\n'
	for i in instrs:
		s += i.disassemble_code()
	s += '#endif\n\n'	
	
	s += '#ifdef SS_GEN_INTERPRETER\n\n'
	for i in instrs:
		s += i.interpret_code()
	s += '#endif\n\n'
	
	return s

s = gen_code()	
print s
open('Generated.h','wt').write(s)