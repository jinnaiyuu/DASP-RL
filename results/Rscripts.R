#####################################
# Input: rom name (e.g. "alien")
compareScores <- function(roms = roms.names[[1]], 
                          actions = c("default","minimal","extended", "extended10", "dasp","dasp.iw1"),
                          smooth = FALSE,
                          optimistic = FALSE,
                          rseeds = c(1)) {
  options = "";
  if (optimistic) {
    options <- "optimistic.init.";
  }
  
  plotOptions = ".";
  if (smooth) {
    plotOptions <- "smooth.";
  }
#  print(roms);
#  print(roms.names);
  for (rom in roms) {
    print(rom);
    p <- ggplot(NULL);
    for (rseed in rseeds) {
      tbl <- buildTable(rom, actions, options = options, rseed = rseed);
      for (a in actions) {
        if (smooth) {
          ytext <- paste("cumsum(", a, ".score)/ID", sep="");
        } else {
          ytext <- paste(a, ".score" ,sep="");
        }
        p <- p + geom_line(data = tbl,aes_string(x = "ID",y=ytext), color = colors[a]);
      }
    }
    filename <- paste(rom, ".", options, "rseed", toString(rseeds), ".", "episode-score.", plotOptions, "pdf", sep="");
    savePlot(p, filename, show = FALSE);
  }
}

#####################################
# Input: rom name (e.g. "alien")
# Compare scores in the function of time = fps * frames.
compareScoresToTime <- function(roms = roms.names[[1]],
                                actions = c("default","minimal","extended", "extended10", "dasp","dasp.iw1"),
                                smooth = FALSE,
                                optimistic = FALSE,
                                rseed = 1) {
  options = "";
  if (optimistic) {
    options <- "optimistic.init.";
  }
  
  plotOptions = ".";
  if (smooth) {
    plotOptions <- "smooth.";
  }
  
  for (rom in roms) {
    tbl <- buildTable(rom, actions, options = options, rseed = rseed);
  #  actions = c("default","minimal","extended", "extended10", "dasp","dasp.iw1");
    # ggplot
    p <- ggplot(data=tbl);
    for (a in actions) {
      xtext <- paste("cumsum(", a, ".frame/", a, ".fps)" , sep="");
  #    ytext <- paste("cumsum(", a, ".score)/ID", sep="");
      if (smooth) {
        ytext <- paste("cumsum(", a, ".score)/ID", sep="");
      } else {
        ytext <- paste(a, ".score", sep="");
      }
      p <- p + geom_line(aes_string(x = xtext,y=ytext), color = colors[a]);
    }
    filename <- paste(rom, ".", options, "rseed", rseed, ".", "time-score.", plotOptions, "pdf", sep="");
    savePlot(p, filename);
  }
}

#####################################
# Input: rom name (e.g. "alien")
compareRAM <- function(roms = roms.names[[1]], actions = c("default", "extended10"),
                       optimistic = FALSE,
                       rseed = 1) {
  options = "";
  if (optimistic) {
    options <- "optimistic.init.";
  }
  
  plotOptions = ".";
#  if (smooth) {
#    plotOptions <- "smooth.";
#  }
  
  for (rom in roms) {
    tbl <- buildTable(rom, 
                      actions = actions, 
                      cls = c("score","frame","fps", "memory"),
                      options = options,
                      rseed = rseed);
    # ggplot
    p <- ggplot(data=tbl);
    for (a in actions) {
      xtext <- paste(a, ".memory", sep="");
      ytext <- paste(a, ".score", sep="");
      print(xtext);
      p <- p + geom_line(aes_string(x=xtext, y=ytext), color = colors[a]);
    }
  
    filename <- paste(rom, ".", options, "rseed", rseed, ".", "ram-score", plotOptions, ".pdf", sep="");
    savePlot(p, filename);
  }
}

#####################################
# Input: Directory name (e.g. default, minimal)
# Read data files to the global variables.
readFiles <- function(conf) {
  tmp <- list.files(path = paste("./", conf, "/", sep=""), pattern = "*");
  for (i in 1:length(tmp)) {
    assign(gsub("-",".",tmp[i]), 
           read.table(file = paste("./", conf, "/", tmp[i], sep=""),header = TRUE), 
           envir = .GlobalEnv);
  }
  print(length(tmp));
}

#####################################
# Input: name of rom
buildTable <- function(rom, 
                       actions = 
                         c("default","minimal","extended", "extended10", "dasp","dasp.iw1"), 
                       cls = c("score","frame","fps"),
                       options = "",
                       rseed = 1) {
  
  lst <- list();
  i = 1;

  for (a in actions) {
#    print(a);
    assign(a, 
           get(paste("Blob_",rom,".bin_", a, ".action.", options, "cfg_rseed", rseed, sep="")));
    lst[[i]] <- get(a);
    i <- i + 1;
  }

  # Prepare the big table which contains all the data.  
#  minsize <- min(sapply(lst, nrow));

  maxsize <- max(sapply(lst, nrow));
  
  tmp <- matrix(NA, ncol = length(cls), nrow = maxsize - nrow(lst[[1]]) );
  colnames(tmp) <- c("score", "frame", "fps");
#  tbl <- lst[[1]][c(1:minsize),cls];
  tbl <- rbind(lst[[1]][,cls], tmp);
#  tbl <- matrix(NA, ncol = length(cls), nrow = maxsize - nrow(lst[[1]]) )
  if (length(actions) > 1) {
    for (i in 2:length(actions)) {
      tmp <- matrix(NA, ncol = length(cls), nrow = maxsize - nrow(lst[[i]]) );
      colnames(tmp) <- c("score", "frame", "fps");
      part <- rbind(lst[[i]][,cls], tmp)
      tbl <- cbind(tbl, part);
    }
  }

  tbl <- cbind(tbl, seq.int(nrow(tbl)));
  
  ns <- vector();
  for (a in actions) {
    for (c in cls) {
      ns <- c(ns, paste(a, ".", c, sep=""));      
    }
  }
  ns <- c(ns, "ID");
  colnames(tbl) <- ns;
#  print(tbl)
  return(tbl);
}

#####################################
# ggplot
savePlot <- function(plot, filename, show = FALSE) {
  pdf(filename);
  print(plot);
  dev.off();
  if (show) {
    print(plot);
  }
}

# Table of method-color. To make it consistent
colors = c("green", "red", "purple", "pink", "blue", "yellow");
names(colors) = c("default","minimal","extended", "extended10", "dasp","dasp.iw1")
#roms.names <- read.table("rom_names")