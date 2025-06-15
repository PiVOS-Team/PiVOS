#include <kernel/call.h>
#include <kernel/log.h>

static kcall_fn kcalls[KCALL_BUFFER_SIZE];

int32_t kcall_register(uint16_t call_id, kcall_fn call) {
    if (call_id >= KCALL_BUFFER_SIZE)
        return KSTATUS_CALL_OUT_OF_BOUNDS;
    if (kcalls[call_id])
        return KSTATUS_CALL_ALREADY_EXISTS;

    kcalls[call_id] = call;
    return call_id;
}

void kcall_unsupported(int64_t *rs) {
    rs[0] = -1;
}

void kcall_dispatch(int64_t *rs, uint64_t esr) {
    uint16_t call_id = (uint16_t)esr;
    if (call_id > KCALL_BUFFER_SIZE) {
        kcall_unsupported(rs);
        return;
    }
    kcall_fn call = kcalls[call_id];
    if (call == 0) {
        kcall_unsupported(rs);
        return;
    }

    call(rs);
}