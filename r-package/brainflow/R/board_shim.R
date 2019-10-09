#' @export
Boards <- function() {
    list(
        Synthetic = c(Id = as.integer(-1)),
        Cyton = c(Id = as.integer(0)),
        Ganglion = c(Id = as.integer(1)),
        CytonDaisy = c(Id = as.integer(2)),
        NovaXR = c(Id = as.integer(3)),
        GanglionWifi = c(Id = as.integer(4)),
        CytonWifi = c(Id = as.integer(5)),
        CytonDaisyWifi = c(Id = as.integer(6))
    )
}

#' @param board_id
#' @param port_name
#' @return BoardShim object
#' @export
get_board_shim_object <- function(board_id, port_name) {
    brainflow$BoardShim(board_id, port_name)
}

#' @return BoardShim class
#' @export
get_board_shim_class <- function() {
    brainflow$BoardShim
}