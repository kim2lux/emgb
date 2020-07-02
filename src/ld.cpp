		{ 0x40, "ld b <- b",std::bind(ld_b_b, this), 0 }, //b
		{ 0x41, "ld b <- c",std::bind(ld_b_c, this), 0 },
		{ 0x42, "ld b <- d",std::bind(ld_b_d, this), 0 },
		{ 0x43, "ld b <- e",std::bind(ld_b_e, this), 0 },
		{ 0x44, "ld b <- h",std::bind(ld_b_h, this), 0 },
		{ 0x45, "ld b <- l",std::bind(ld_b_l, this), 0 },
		{ 0x46, "ld b <- hl",std::bind(ld_b_hl, this), 0 },
		{ 0x47, "ld b <- a",std::bind(ld_b_a, this), 0 },

		{ 0x48, "ld c <- b",std::bind(ld_c_b, this), 0 }, //c
		{ 0x49, "ld c <- c",std::bind(ld_c_c, this), 0 },
		{ 0x4a, "ld c <- d",std::bind(ld_c_d, this), 0 },
		{ 0x4b, "ld c <- e",std::bind(ld_c_e, this), 0 },
		{ 0x4c, "ld c <- h",std::bind(ld_c_h, this), 0 },
		{ 0x4d, "ld c <- l",std::bind(ld_c_l, this), 0 },
		{ 0x4e, "ld c <- hl",std::bind(ld_c_hl, this), 0 },
		{ 0x4f, "ld c <- a",std::bind(ld_c_a, this), 0 },

		{ 0x50, "ld d <- b",std::bind(ld_d_b, this), 0 }, //D
		{ 0x51, "ld d <- c",std::bind(ld_d_c, this), 0 },
		{ 0x52, "ld d <- d",std::bind(ld_d_d, this), 0 },
		{ 0x53, "ld d <- e",std::bind(ld_d_e, this), 0 },
		{ 0x54, "ld d <- h",std::bind(ld_d_h, this), 0 },
		{ 0x55, "ld d <- l",std::bind(ld_d_l, this), 0 },
		{ 0x56, "ld d <- hl",std::bind(ld_d_hl, this), 0 },
		{ 0x57, "ld d <- a",std::bind(ld_d_a, this), 0 },

		{ 0x58, "ld e <- b",std::bind(ld_e_b, this), 0 }, //E
		{ 0x59, "ld e <- c",std::bind(ld_e_c, this), 0 },
		{ 0x5a, "ld e <- d",std::bind(ld_e_d, this), 0 },
		{ 0x5b, "ld e <- e",std::bind(ld_e_e, this), 0 },
		{ 0x5c, "ld e <- h",std::bind(ld_e_h, this), 0 },
		{ 0x5d, "ld e <- l",std::bind(ld_e_l, this), 0 },
		{ 0x5e, "ld e <- hl",std::bind(ld_e_hl, this), 0 },
		{ 0x5f, "ld e <- a",std::bind(ld_e_a, this), 0 },

		{ 0x60, "ld h <- b",std::bind(ld_h_b, this), 0 }, //H
		{ 0x61, "ld h <- c",std::bind(ld_h_c, this), 0 },
		{ 0x62, "ld h <- d",std::bind(ld_h_d, this), 0 },
		{ 0x63, "ld h <- e",std::bind(ld_h_e, this), 0 },
		{ 0x64, "ld h <- h",std::bind(ld_h_h, this), 0 },
		{ 0x65, "ld h <- l",std::bind(ld_h_l, this), 0 },
		{ 0x66, "ld h <- hl",std::bind(ld_h_hl, this), 0 },
		{ 0x67, "ld h <- a",std::bind(ld_h_a, this), 0 },

		{ 0x68, "ld l <- b",std::bind(ld_l_b, this), 0 }, //L
		{ 0x69, "ld l <- c",std::bind(ld_l_c, this), 0 },
		{ 0x6a, "ld l <- d",std::bind(ld_l_d, this), 0 },
		{ 0x6b, "ld l <- e",std::bind(ld_l_e, this), 0 },
		{ 0x6c, "ld l <- h",std::bind(ld_l_h, this), 0 },
		{ 0x6d, "ld l <- l",std::bind(ld_l_l, this), 0 },
		{ 0x6e, "ld l <- hl",std::bind(ld_l_hl, this), 0 },
		{ 0x6f, "ld l <- a",std::bind(ld_l_a, this), 0 },

		{ 0x70, "ld hl <- b",std::bind(ld_hl_b, this), 0 }, //(HL)
		{ 0x71, "ld hl <- c",std::bind(ld_hl_c, this), 0 },
		{ 0x72, "ld hl <- d",std::bind(ld_hl_d, this), 0 },
		{ 0x73, "ld hl <- e",std::bind(ld_hl_e, this), 0 },
		{ 0x74, "ld hl <- h",std::bind(ld_hl_h, this), 0 },
		{ 0x75, "ld hl <- l",std::bind(ld_hl_l, this), 0 },
		{ 0x76, "halt proc", halt_proc, this), 0 },
		{ 0x77, "ld hl <- a",std::bind(ld_hl_a, this), 0 },

		{ 0x78, "ld a <- b",std::bind(ld_a_b, this), 0 }, //A
		{ 0x79, "ld a <- c",std::bind(ld_a_c, this), 0 },
		{ 0x7a, "ld a <- d",std::bind(ld_a_d, this), 0 },
		{ 0x7b, "ld a <- e",std::bind(ld_a_e, this), 0 },
		{ 0x7c, "ld a <- h",std::bind(ld_a_h, this), 0 },
		{ 0x7d, "ld a <- l",std::bind(ld_a_l, this), 0 },
		{ 0x7e, "ld a <- hl",std::bind(ld_a_hl, this), 0 },
		{ 0x7f, "ld a <- a",std::bind(ld_a_a, this), 0 },